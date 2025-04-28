#include "magasin_mainwindow.h"
#include "ui_magasin_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlError>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QPrinter>
#include <QDesktopServices>

MagasinMainWindow::MagasinMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MagasinMainWindow) {
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(500);

    // Configuration de la tableWidget pour l'affichage des magasins
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"ID MAGASIN", "NOM MAGASIN", "TELEPHONE LOCATAIRE", "EMAIL DE MAGASIN", "PRIX DE LOCATION", "REFERENCE"});
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Connexions aux différents boutons et actions
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MagasinMainWindow::on_comboBox_currentIndexChanged);
    connect(ui->sup, &QPushButton::clicked, this, &MagasinMainWindow::on_supprimerMagasin_clicked);
    connect(ui->modifierButton, &QPushButton::clicked, this, &MagasinMainWindow::on_modifierMagasin_clicked);
    connect(ui->rechercheButton, &QPushButton::clicked, this, &MagasinMainWindow::on_rechercheButton_clicked);
    connect(ui->ajout, &QPushButton::clicked, this, &MagasinMainWindow::on_ajouterMagasin_clicked);
    connect(ui->ajout, &QPushButton::clicked, this, &MagasinMainWindow::mettreAJourComboBox);
    connect(ui->triNomButton, &QPushButton::clicked, this, &MagasinMainWindow::on_triNomButton_clicked);
    connect(ui->triPrixButton, &QPushButton::clicked, this, &MagasinMainWindow::on_triPrixButton_clicked);
    connect(ui->afficherButton, &QPushButton::clicked, this, &MagasinMainWindow::on_afficherButton_clicked);
    connect(ui->fermerButton, &QPushButton::clicked, this, &MagasinMainWindow::on_fermerButton_clicked);
    connect(ui->statistiquesButton, &QPushButton::clicked, this, &MagasinMainWindow::on_statistiquesButton_clicked);
    connect(ui->afficherButton, &QPushButton::clicked, this, &MagasinMainWindow::mettreAJourComboBox);
    connect(ui->btnEnvoyerRappels, &QPushButton::clicked, this, &MagasinMainWindow::envoyerTousLesRappels);
    connect(ui->AA, &QPushButton::clicked, this, &MagasinMainWindow::appliquerSolde);
    connect(ui->pushButtonRetirerSolde, &QPushButton::clicked, this, &MagasinMainWindow::annulerSolde);
    connect(ui->exporterButton, &QPushButton::clicked, this, [=]() {
        QString defaultName = "Liste_Magasins_" + QDate::currentDate().toString("yyyyMMdd") + ".pdf";
        QString fileName = QFileDialog::getSaveFileName(
            this, "Enregistrer en PDF", QDir::homePath() + "/" + defaultName, "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
                fileName += ".pdf";
            }
            exportToPDF(fileName);
        }
    });

    // Load magasins from database
    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT * FROM MAGASINS");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString nom = query.value("nom").toString();
        QString telephone = query.value("telephone").toString();
        QString email = query.value("email").toString();
        double prix = query.value("prix").toDouble();
        QString reference = query.value("reference").toString();
        Magasin m(id, nom, telephone, email, prix, reference);
        magasins.append(m);
    }

    afficherMagasinsDansTable();
    mettreAJourComboBox();
}

MagasinMainWindow::~MagasinMainWindow() {
    delete ui;
    delete timer;
}

void MagasinMainWindow::on_ajouterMagasin_clicked() {
    QString idText = ui->ID_MAGASIN->text().trimmed();
    QString nom = ui->NOM_MAGASIN->text().trimmed();
    QString telephone = ui->TELEPHONE_LOCATAIRE->text().trimmed();
    QString email = ui->EMAIL_DE_MAGASIN->text().trimmed();
    QString prixText = ui->PRIX_DE_LOCATION->text().trimmed();
    QString reference = ui->REFERENCE->text().trimmed();

    if (idText.isEmpty() || nom.isEmpty() || telephone.isEmpty() || email.isEmpty() || prixText.isEmpty() || reference.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    int id = idText.toInt();
    double prix = prixText.toDouble();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID du magasin doit être un nombre positif !");
        return;
    }
    if (prix <= 0) {
        QMessageBox::warning(this, "Erreur", "Le prix de location doit être un nombre positif !");
        return;
    }

    if (telephone.length() != 10 || !telephone.toLongLong()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir exactement 10 chiffres !");
        return;
    }

    QRegularExpression emailRegex("^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'adresse email est invalide !");
        return;
    }

    for (const Magasin& magasin : magasins) {
        if (magasin.get_ID_MAGASIN() == id) {
            QMessageBox::warning(this, "Erreur", "L'ID du magasin existe déjà !");
            return;
        }
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("INSERT INTO MAGASINS (id, nom, telephone, email, prix, reference) "
                  "VALUES (:id, :nom, :telephone, :email, :prix, :reference)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":email", email);
    query.bindValue(":prix", prix);
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'insertion dans la base de données:\n" + query.lastError().text());
        return;
    }

    Magasin nouveauMagasin(id, nom, telephone, email, prix, reference);
    magasins.append(nouveauMagasin);

    mettreAJourComboBox();
    afficherMagasinsDansTable();

    QMessageBox::information(this, "Succès", "Magasin ajouté avec succès !");

    ui->ID_MAGASIN->clear();
    ui->NOM_MAGASIN->clear();
    ui->TELEPHONE_LOCATAIRE->clear();
    ui->EMAIL_DE_MAGASIN->clear();
    ui->PRIX_DE_LOCATION->clear();
    ui->REFERENCE->clear();
}

void MagasinMainWindow::on_supprimerMagasin_clicked() {
    int id = ui->comboBox->currentText().toInt();

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("DELETE FROM MAGASINS WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        mettreAJourComboBox();
        afficherMagasinsDansTable();
        QMessageBox::information(this, "Succès", "Suppression réussie !");
        ui->ID_MAGASIN->clear();
        ui->NOM_MAGASIN->clear();
        ui->TELEPHONE_LOCATAIRE->clear();
        ui->EMAIL_DE_MAGASIN->clear();
        ui->PRIX_DE_LOCATION->clear();
        ui->REFERENCE->clear();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de supprimer le magasin.");
    }
}

void MagasinMainWindow::on_modifierMagasin_clicked() {
    int id = ui->comboBox->currentText().toInt();

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("UPDATE MAGASINS SET "
                  "nom = :nom, telephone = :telephone, email = :email, prix = :prix, reference = :reference "
                  "WHERE id = :id");
    query.bindValue(":nom", ui->NOM_MAGASIN->text());
    query.bindValue(":telephone", ui->TELEPHONE_LOCATAIRE->text());
    query.bindValue(":email", ui->EMAIL_DE_MAGASIN->text());
    query.bindValue(":prix", ui->PRIX_DE_LOCATION->text().toDouble());
    query.bindValue(":reference", ui->REFERENCE->text());
    query.bindValue(":id", id);

    if (query.exec()) {
        mettreAJourComboBox();
        afficherMagasinsDansTable();
        QMessageBox::information(this, "Succès", "Modification réussie !");
        ui->ID_MAGASIN->clear();
        ui->NOM_MAGASIN->clear();
        ui->TELEPHONE_LOCATAIRE->clear();
        ui->EMAIL_DE_MAGASIN->clear();
        ui->PRIX_DE_LOCATION->clear();
        ui->REFERENCE->clear();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de modifier le magasin.");
    }
}

void MagasinMainWindow::on_rechercheButton_clicked() {
    QString nomRecherche = ui->recherche->text().trimmed();
    ui->tableWidget->setRowCount(0);

    if (nomRecherche.isEmpty()) {
        afficherMagasinsDansTable();
        return;
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT * FROM MAGASINS WHERE nom LIKE :nomRecherche");
    query.bindValue(":nomRecherche", "%" + nomRecherche + "%");

    bool trouve = false;
    if (query.exec()) {
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("telephone").toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("email").toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("prix").toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("reference").toString()));
            trouve = true;
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de rechercher le magasin dans la base de données.");
    }

    if (!trouve) {
        afficherMagasinsDansTable();
        QMessageBox::information(this, "Info", "Aucun magasin avec ce nom. Affichage de tous les magasins.");
    }
}

void MagasinMainWindow::on_triNomButton_clicked() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT * FROM MAGASINS ORDER BY nom ASC");
    if (query.exec()) {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("telephone").toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("email").toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("prix").toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("reference").toString()));
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les magasins triés par nom.");
    }
}

void MagasinMainWindow::on_triPrixButton_clicked() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT * FROM MAGASINS ORDER BY prix ASC");
    if (query.exec()) {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        while (query.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("telephone").toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("email").toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("prix").toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("reference").toString()));
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les magasins triés par prix.");
    }
}

void MagasinMainWindow::on_afficherButton_clicked() {
    afficherMagasinsDansTable();
}

void MagasinMainWindow::on_fermerButton_clicked() {
    close();
}

void MagasinMainWindow::on_statistiquesButton_clicked() {
    double moyenne = calculerMoyennePrix();
    double min = calculerPrixMinimum();
    double max = calculerPrixMaximum();
    double ecartType = calculerEcartTypePrix();
    QString message = QString("📊 Statistiques des prix :\n\n")
                      + "➤ Moyenne : " + QString::number(moyenne, 'f', 2) + " DT\n"
                      + "➤ Minimum : " + QString::number(min, 'f', 2) + " DT\n"
                      + "➤ Maximum : " + QString::number(max, 'f', 2) + " DT\n"
                      + "➤ Écart-type : " + QString::number(ecartType, 'f', 2) + " DT";
    QMessageBox::information(this, "Statistiques des prix", message);
}

void MagasinMainWindow::on_comboBox_currentIndexChanged(int index) {
    if (index >= 0) {
        int idMagasin = ui->comboBox->itemData(index).toInt();
        QSqlQuery query(QSqlDatabase::database());
        query.prepare("SELECT id, nom, telephone, email, prix, reference FROM MAGASINS WHERE id = :idMagasin");
        query.bindValue(":idMagasin", idMagasin);
        if (query.exec() && query.next()) {
            ui->ID_MAGASIN->setText(query.value("id").toString());
            ui->NOM_MAGASIN->setText(query.value("nom").toString());
            ui->TELEPHONE_LOCATAIRE->setText(query.value("telephone").toString());
            ui->EMAIL_DE_MAGASIN->setText(query.value("email").toString());
            ui->PRIX_DE_LOCATION->setText(query.value("prix").toString());
            ui->REFERENCE->setText(query.value("reference").toString());
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de récupérer les informations du magasin.");
        }
    }
}

void MagasinMainWindow::afficherMagasinsDansTable() {
    ui->tableWidget->setRowCount(0);
    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT * FROM MAGASINS");
    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("nom").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("telephone").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("email").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(query.value("prix").toDouble(), 'f', 2)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("reference").toString()));
    }
    ui->tableWidget->resizeColumnsToContents();
}

void MagasinMainWindow::mettreAJourComboBox() {
    ui->comboBox->clear();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT id FROM MAGASINS");
    if (query.exec()) {
        while (query.next()) {
            int idMagasin = query.value("id").toInt();
            ui->comboBox->addItem(QString::number(idMagasin), idMagasin);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les ID des magasins.");
    }
}

double MagasinMainWindow::calculerMoyennePrix() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT AVG(prix) FROM MAGASINS");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double MagasinMainWindow::calculerPrixMinimum() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT MIN(prix) FROM MAGASINS");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double MagasinMainWindow::calculerPrixMaximum() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT MAX(prix) FROM MAGASINS");
    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

double MagasinMainWindow::calculerEcartTypePrix() {
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT AVG(prix) FROM MAGASINS");
    double moyenne = 0.0;
    if (query.exec() && query.next()) {
        moyenne = query.value(0).toDouble();
    } else {
        return 0.0;
    }

    query.prepare("SELECT (prix - :moyenne) * (prix - :moyenne) FROM MAGASINS");
    query.bindValue(":moyenne", moyenne);
    double somme = 0.0;
    int count = 0;
    if (query.exec()) {
        while (query.next()) {
            somme += query.value(0).toDouble();
            count++;
        }
    }
    if (count > 1) {
        double variance = somme / count;
        return sqrt(variance);
    }
    return 0.0;
}

void MagasinMainWindow::exportToPDF(const QString &fileName) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 20, 15, 15));

    QString html = "<div style='display: flex; margin-bottom: 25px; padding-bottom: 15px; border-bottom: 2px solid #2E86C1;'>"
                   "<img src=':/imgs/logo.png' width='120' style='margin-right: 30px;'/>"
                   "<div style='flex-grow: 1; text-align: center;'>"
                   "<h1 style='color: #2E86C1; margin: 0; font-family: Arial;'>Liste des Magasins</h1>"
                   "<p style='color: #6C757D; font-size: 12px; margin-top: 3px;'>"
                   "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>"
                   "</div></div>";

    html += "<style>"
            "table { margin: 0 auto; width: 98%; border-collapse: collapse; border: 1px solid #ddd; }"
            "th, td { padding: 10px; border: 1px solid #ccc; text-align: center; font-family: Arial; font-size: 11pt; }"
            "td:nth-child(2) { text-align: left; }"
            "th { background-color: #f8f9fa; color: #2E86C1; font-size: 12pt; }"
            "</style>";

    html += "<table><tr>";
    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        QTableWidgetItem *header = ui->tableWidget->horizontalHeaderItem(col);
        html += "<th>" + (header ? header->text() : QString("Column %1").arg(col+1)) + "</th>";
    }
    html += "</tr>";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QString data = item ? item->text() : "";
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void MagasinMainWindow::envoyerTousLesRappels() {
    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT id, nom, reference, email, prix FROM MAGASINS");
    int total = 0;
    while (query.next()) {
        int id = query.value("id").toInt();
        QString nomMagasin = query.value("nom").toString();
        QString reference = query.value("reference").toString();
        QString email = query.value("email").toString();
        double prix = query.value("prix").toDouble();
        if (!email.isEmpty()) {
            envoyerEmailRappel(email, id, nomMagasin, reference, prix);
            total++;
        }
    }
    QMessageBox::information(this, "Rappels envoyés",
                             QString("✅ %1 rappels envoyés avec succès.").arg(total));
}

void MagasinMainWindow::envoyerEmailRappel(QString email, int id, QString nomMagasin, QString reference, double prix) {
    QNetworkRequest request(QUrl("https://api.sendgrid.com/v3/mail/send"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer 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");

    QString contenuHtml = QString(R"(
    <html>
    <head>
      <style>
        .container {
          font-family: Arial, sans-serif;
          background-color: #f9f9f9;
          padding: 20px;
          border-radius: 10px;
          color: #333;
        }
        .header {
          font-size: 20px;
          font-weight: bold;
          margin-bottom: 15px;
        }
        .info {
          margin: 10px 0;
        }
        .footer {
          margin-top: 20px;
          font-size: 12px;
          color: #777;
        }
      </style>
    </head>
    <body>
      <div class="container">
        <div class="header">📌 Rappel - Paiement de la location</div>
        <div class="info">🆔 <strong>ID :</strong> %1</div>
        <div class="info">🏬 <strong>Nom du magasin :</strong> %2</div>
        <div class="info">🔖 <strong>Référence :</strong> %3</div>
        <div class="info">💰 <strong>Prix de location :</strong> %4 TND</div>
        <div class="info">📅 <strong>Date du rappel :</strong> %5</div>
        <p>Veuillez régulariser votre situation dans les plus brefs délais afin d'éviter toute interruption.</p>
        <div class="footer">
          Merci pour votre confiance.<br>
          Service de Gestion des Magasins.
        </div>
      </div>
    </body>
    </html>
    )").arg(id).arg(nomMagasin).arg(reference).arg(prix).arg(QDate::currentDate().toString("dd/MM/yyyy"));

    QJsonObject mail;
    mail["personalizations"] = QJsonArray{
        QJsonObject{
            {"to", QJsonArray{QJsonObject{{"email", email}}}},
            {"subject", "📩 Rappel de paiement de location"}
        }
    };
    mail["from"] = QJsonObject{{"email", "chrifsmaya740@gmail.com"}};
    mail["content"] = QJsonArray{
        QJsonObject{
            {"type", "text/html"},
            {"value", contenuHtml}
        }
    };

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->post(request, QJsonDocument(mail).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "✅ Email envoyé avec succès à:" << email;
        } else {
            qDebug() << "❌ Erreur d'envoi:" << reply->errorString();
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MagasinMainWindow::appliquerSolde() {
    int id = ui->ID_MAGASIN->text().toInt();
    double pourcentage = 0;
    if (ui->checkBox10_2->isChecked()) {
        pourcentage = 10;
    } else if (ui->checkBox20_2->isChecked()) {
        pourcentage = 20;
    } else if (ui->checkBox30_2->isChecked()) {
        pourcentage = 30;
    } else {
        QMessageBox::warning(this, "Sélection de solde", "Veuillez sélectionner un pourcentage de solde.");
        return;
    }

    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT prix FROM MAGASINS WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        double prix = query.value(0).toDouble();
        double prixSolde = prix - (prix * (pourcentage / 100.0));
        QSqlQuery updateQuery(QSqlDatabase::database());
        updateQuery.prepare("UPDATE MAGASINS SET prix_ancien = :ancien, prix = :nouveau WHERE id = :id");
        updateQuery.bindValue(":ancien", prix);
        updateQuery.bindValue(":nouveau", prixSolde);
        updateQuery.bindValue(":id", id);
        if (updateQuery.exec()) {
            QMessageBox::information(this, "Solde", "✅ Prix mis à jour avec succès !");
        } else {
            QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de la mise à jour du prix.");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "ID introuvable !");
    }
}

void MagasinMainWindow::annulerSolde() {
    int id = ui->ID_MAGASIN->text().toInt();
    QSqlQuery query(QSqlDatabase::database());
    query.prepare("SELECT prix_ancien FROM MAGASINS WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        double prixAncien = query.value(0).toDouble();
        QSqlQuery updateQuery(QSqlDatabase::database());
        updateQuery.prepare("UPDATE MAGASINS SET prix = :prix, prix_ancien = NULL WHERE id = :id");
        updateQuery.bindValue(":prix", prixAncien);
        updateQuery.bindValue(":id", id);
        if (updateQuery.exec()) {
            QMessageBox::information(this, "Solde retiré", "✅ Ancien prix restauré !");
        } else {
            QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de la restauration du prix.");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Aucun prix ancien trouvé !");
    }
}
