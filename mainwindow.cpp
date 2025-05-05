#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fournisseur.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QFileDialog>
#include <QDebug>
#include <QSerialPort>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(new QSerialPort(this))
    , lastUid("")
{
    ui->setupUi(this);
    connect(ui->pb_id, &QPushButton::clicked, this, &MainWindow::on_pb_id_clicked);
    ui->le_id->setValidator(new QIntValidator(0, 999999, this));
    ui->tab_fournisseur->setModel(f.afficher());
    updateStatsChart();

    // Arduino initialization
    setupSerialPort();
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
}

MainWindow::~MainWindow()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
    delete ui;
}

void MainWindow::setupSerialPort()
{
    serialPort->setPortName("COM7");
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Port série ouvert avec succès en lecture/écriture:" << serialPort->portName();
    } else {
        qDebug() << "Erreur lors de l'ouverture du port série:" << serialPort->errorString();
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série: " + serialPort->errorString() +
                                                  "\nVérifiez que l'Arduino est connecté à COM4 et que le port n'est pas utilisé.");
    }
}

bool MainWindow::isValidRFID(const QString &uid)
{
    // Vérifie si l'UID est un format hexadécimal valide (par ex., "8C E2 86 33")
    QRegularExpression re("^[0-9A-Fa-f]{2}\\s[0-9A-Fa-f]{2}\\s[0-9A-Fa-f]{2}\\s[0-9A-Fa-f]{2}$");
    return re.match(uid).hasMatch();
}

void MainWindow::readSerialData()
{
    while (serialPort->canReadLine()) {
        QString data = QString(serialPort->readLine()).trimmed();
        qDebug() << "Données reçues:" << data;
        // Ignorer les messages non-UID comme "Accès autorisé !" ou "Accès refusé !"
        if (data == "Accès autorisé !" || data == "Accès refusé !" || data.startsWith("Erreur :")) {
            continue;
        }
        if (isValidRFID(data) && data != lastUid) { // Vérifie si c'est un UID valide et nouveau
            lastUid = data;
            checkRFID(data);
        }
    }
}

void MainWindow::checkRFID(const QString &uid)
{
    QSqlQuery query;
    query.prepare("SELECT ID_FOURNISSEUR, NOM_FOURNISSEUR FROM FOURNISSEURS WHERE RFID_UID = :uid AND AUTORISATION = 'Y'");
    query.bindValue(":uid", uid);

    if (query.exec()) {
        if (query.next()) {
            int id_fournisseur = query.value("ID_FOURNISSEUR").toInt();
            QString nom_fournisseur = query.value("NOM_FOURNISSEUR").toString().left(10); // Limiter à 10 caractères

            // Préparer le message pour la QMessageBox et le LCD
            QString message = QString("Fournisseur: %1 ID: %2").arg(nom_fournisseur).arg(id_fournisseur);

            // Affichage dans la QMessageBox
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Accès autorisé");
            msgBox.setText(message);
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStyleSheet(
                "QMessageBox {"
                "    background-color: #e6f3e6;"
                "    border: 2px solid #4CAF50;"
                "    border-radius: 10px;"
                "}"
                "QLabel {"
                "    color: #2e7d32;"
                "    font-family: 'Arial';"
                "    font-size: 16px;"
                "    font-weight: bold;"
                "}"
                "QPushButton {"
                "    background-color: #4CAF50;"
                "    color: white;"
                "    border-radius: 5px;"
                "    padding: 8px 16px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #45a049;"
                "}"
                );
            msgBox.exec();

            // Envoi du même message au LCD via le port série
            if (serialPort->isOpen()) {
                qDebug() << "Port série ouvert. Envoi au LCD:" << message;
                QByteArray data = message.toUtf8() + "\n";
                qint64 bytesWritten = serialPort->write(data);
                if (bytesWritten == data.size() && serialPort->flush()) {
                    qDebug() << "Message envoyé avec succès. Octets écrits:" << bytesWritten;
                } else {
                    qDebug() << "Échec de l'envoi du message. Octets écrits:" << bytesWritten;
                }
            } else {
                qDebug() << "Port série non ouvert. Erreur:" << serialPort->errorString();
            }

            f.logAccess(id_fournisseur, uid, true);
        } else {
            // Préparer le message pour accès refusé
            QString message = "vous n'etes pas autoriser de passer.";

            // Affichage dans la QMessageBox
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Accès refusé");
            msgBox.setText(message);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet(
                "QMessageBox {"
                "    background-color: #ffebee;"
                "    border: 2px solid #d32f2f;"
                "    border-radius: 10px;"
                "}"
                "QLabel {"
                "    color: #b71c1c;"
                "    font-family: 'Arial';"
                "    font-size: 16px;"
                "    font-weight: bold;"
                "}"
                "QPushButton {"
                "    background-color: #d32f2f;"
                "    color: white;"
                "    border-radius: 5px;"
                "    padding: 8px 16px;"
                "    font-size: 14px;"
                "}"
                "QPushButton:hover {"
                "    background-color: #c62828;"
                "}"
                );
            msgBox.exec();

            // Envoi au LCD
            if (serialPort->isOpen()) {
                qDebug() << "Port série ouvert. Envoi au LCD:" << message;
                QByteArray data = message.toUtf8() + "\n";
                qint64 bytesWritten = serialPort->write(data);
                if (bytesWritten == data.size() && serialPort->flush()) {
                    qDebug() << "Message envoyé avec succès. Octets écrits:" << bytesWritten;
                } else {
                    qDebug() << "Échec de l'envoi du message. Octets écrits:" << bytesWritten;
                }
            } else {
                qDebug() << "Port série non ouvert. Erreur:" << serialPort->errorString();
            }

            f.logAccess(0, uid, false);
        }
    } else {
        qDebug() << "Erreur lors de la requête RFID:" << query.lastError().text();
        QString message = "Erreur lors de la vérification RFID.";

        // Affichage dans la QMessageBox
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Erreur");
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        // Envoi au LCD
        if (serialPort->isOpen()) {
            qDebug() << "Port série ouvert. Envoi au LCD:" << message;
            QByteArray data = message.toUtf8() + "\n";
            qint64 bytesWritten = serialPort->write(data);
            if (bytesWritten == data.size() && serialPort->flush()) {
                qDebug() << "Message envoyé avec succès. Octets écrits:" << bytesWritten;
            } else {
                qDebug() << "Échec de l'envoi du message. Octets écrits:" << bytesWritten;
            }
        } else {
            qDebug() << "Port série non ouvert. Erreur:" << serialPort->errorString();
        }
    }
}

QString MainWindow::generateCaptchaText(int length)
{
    const QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString randomString;
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

bool MainWindow::verifyCaptcha()
{
    QString captchaText = generateCaptchaText(6);
    bool ok;
    QString userInput = QInputDialog::getText(this, "Vérification Captcha",
                                              "Veuillez saisir le texte ci-dessous :\n" + captchaText,
                                              QLineEdit::Normal, "", &ok);
    if (ok && !userInput.isEmpty()) {
        return userInput == captchaText;
    }
    return false;
}

void MainWindow::on_pb_ajouter_clicked()
{
    int id_fournisseur = ui->le_id->text().toInt();
    QString nom_fournisseur = ui->le_nom_fournisseur->text();
    QString adresse = ui->le_adresse->text();
    QString telephone_fournisseur = ui->le_telephone_fournisseur->text();
    QString email_fournisseur = ui->le_email_fournisseur->text().trimmed();
    QString type_fournisseur = ui->le_type_fournisseur->text().trimmed();

    bool erreur = false;

    if (type_fournisseur != "stratégiques" && type_fournisseur != "sous-traitants") {
        ui->label_erreur_type->setText("Type de fournisseur inexistant, soit stratégiques, sous-traitants.");
        erreur = true;
    } else {
        ui->label_erreur_type->clear();
    }

    if (!email_fournisseur.endsWith("@gmail.com")) {
        ui->label_erreur_email->setText("L'adresse email doit se terminer par @gmail.com.");
        erreur = true;
    } else {
        ui->label_erreur_email->clear();
    }

    if (erreur) {
        return;
    }

    if (!verifyCaptcha()) {
        QMessageBox::warning(this, "Captcha incorrect",
                             "Le texte saisi ne correspond pas au captcha. Veuillez réessayer.");
        return;
    }

    fournisseur f(id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, type_fournisseur);
    bool test = f.ajouter();
    if (test) {
        ui->tab_fournisseur->setModel(f.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué."), QMessageBox::Ok);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Ajout non effectué."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_supprimer_clicked()
{
    int id_fournisseur = ui->le_id_2->text().toInt();
    fournisseur f;
    if (f.supprimer(id_fournisseur)) {
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"), QObject::tr("Fournisseur supprimé."), QMessageBox::Ok);
        ui->tab_fournisseur->setModel(f.afficher());
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Suppression échouée."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_modifier_clicked()
{
    int id_fournisseur = ui->le_id_5->text().toInt();
    QString nom_fournisseur = ui->le_nom_fournisseur_3->text();
    QString adresse = ui->le_adresse_3->text();
    QString telephone_fournisseur = ui->le_telephone_3->text();
    QString email_fournisseur = ui->le_email_fournisseur_3->text().trimmed();
    QString type_fournisseur = ui->le_type_fournisseur_3->text().trimmed();

    // Vérification des champs vides
    if (id_fournisseur == 0 || nom_fournisseur.isEmpty() || adresse.isEmpty() ||
        telephone_fournisseur.isEmpty() || email_fournisseur.isEmpty() || type_fournisseur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    bool erreur = false;

    // Vérification du type de fournisseur
    if (type_fournisseur != "stratégiques" && type_fournisseur != "sous-traitants") {
        ui->label_erreur_type_3->setText("Type de fournisseur inexistant, soit stratégiques, sous-traitants.");
        erreur = true;
    } else {
        ui->label_erreur_type_3->clear();
    }

    // Vérification de l'email
    if (!email_fournisseur.endsWith("@gmail.com")) {
        ui->label_erreur_email_3->setText("L'adresse email doit se terminer par @gmail.com.");
        erreur = true;
    } else {
        ui->label_erreur_email_3->clear();
    }

    // Vérification des doublons pour ID_FOURNISSEUR
    QSqlQuery idCheckQuery;
    idCheckQuery.prepare("SELECT COUNT(*) FROM FOURNISSEURS WHERE ID_FOURNISSEUR = :id_fournisseur AND ID_FOURNISSEUR != :current_id");
    idCheckQuery.bindValue(":id_fournisseur", id_fournisseur);
    idCheckQuery.bindValue(":current_id", id_fournisseur);
    if (idCheckQuery.exec() && idCheckQuery.next() && idCheckQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Erreur", "Cet ID de fournisseur existe déjà.");
        erreur = true;
    }

    // Vérification des doublons pour EMAIL_FOURNISSEUR
    QSqlQuery emailCheckQuery;
    emailCheckQuery.prepare("SELECT COUNT(*) FROM FOURNISSEURS WHERE EMAIL_FOURNISSEUR = :email_fournisseur AND ID_FOURNISSEUR != :current_id");
    emailCheckQuery.bindValue(":email_fournisseur", email_fournisseur);
    emailCheckQuery.bindValue(":current_id", id_fournisseur);
    if (emailCheckQuery.exec() && emailCheckQuery.next() && emailCheckQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Erreur", "Cet email est déjà utilisé par un autre fournisseur.");
        erreur = true;
    }

    if (erreur) {
        return;
    }

    if (f.modifier(id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, type_fournisseur)) {
        QMessageBox::information(this, "Succès", "Modification réussie.");
        ui->tab_fournisseur->setModel(f.afficher());
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::updateStatsChart()
{
    QMap<QString, int> stats = f.getStatsByType();
    QPieSeries *series = new QPieSeries();

    int total = 0;
    for (int count : stats) total += count;

    if (total == 0) {
        QMessageBox::information(this, "Info", "Aucun fournisseur trouvé.");
        return;
    }

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QString type = it.key();
        qreal percentage = (it.value() * 100.0) / total;
        QPieSlice *slice = series->append(type, it.value());
        slice->setLabel(QString("%1 (%2%)").arg(type).arg(percentage, 0, 'f', 1));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des fournisseurs par type");
    chart->legend()->setAlignment(Qt::AlignBottom);

    if (ui->chartView->chart())
        delete ui->chartView->chart();

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::on_Sp_Button_ExportPDF_clicked()
{
    f.printPDF();
}

void MainWindow::on_pb_id_clicked()
{
    ui->tab_fournisseur->setModel(f.afficherTriParId());
}

void MainWindow::on_pb_tri_parnom_clicked()
{
    ui->tab_fournisseur->setModel(f.trierParNom());
}

void MainWindow::on_lineEditRecherche_textChanged(const QString &text)
{
    ui->tab_fournisseur->setModel(f.rechercherParNom(text));
}

void MainWindow::on_pb_refresh_stats_clicked()
{
    updateStatsChart();
}

void MainWindow::on_pb_importer_clicked()
{
    if (!verifyCaptcha()) {
        QMessageBox::warning(this, "Captcha incorrect",
                             "Le texte saisi ne correspond pas au captcha. Veuillez réessayer.");
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Importer des fournisseurs",
        "",
        "Fichiers CSV (*.csv)"
        );

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Annulé", "Aucun fichier sélectionné.");
        return;
    }

    if (f.importerFournisseurs(filePath)) {
        ui->tab_fournisseur->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Fournisseurs importés avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'importation. Vérifiez le fichier et les journaux de débogage.");
    }
}

