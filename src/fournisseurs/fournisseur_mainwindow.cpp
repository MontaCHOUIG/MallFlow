#include "fournisseur_mainwindow.h"
#include "ui_fournisseur_mainwindow.h"
#include "fournisseur.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QFileDialog>

FournisseurMainWindow::FournisseurMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FournisseurMainWindow)
{
    ui->setupUi(this);

    // Connexions des boutons de navigation
    connect(ui->employes, &QPushButton::clicked, this, &FournisseurMainWindow::on_btnEmployees_clicked);
    connect(ui->sponsors, &QPushButton::clicked, this, &FournisseurMainWindow::on_btnSponsors_clicked);
    connect(ui->services, &QPushButton::clicked, this, &FournisseurMainWindow::on_btnServices_clicked);
    connect(ui->evenement, &QPushButton::clicked, this, &FournisseurMainWindow::on_btnEvents_clicked);
    connect(ui->magasins, &QPushButton::clicked, this, &FournisseurMainWindow::on_btnStores_clicked);

    // connect logout signal
    connect(ui->button_logout, &QPushButton::clicked, this, &FournisseurMainWindow::on_button_logout_clicked);

    updateAuthUserLabel();

    connect(ui->pb_id, &QPushButton::clicked, this, &FournisseurMainWindow::on_pb_id_clicked);
    ui->le_id->setValidator(new QIntValidator(0, 999999, this));
    ui->tab_fournisseur->setModel(f.afficher());
    updateStatsChart();
}



FournisseurMainWindow::~FournisseurMainWindow()
{
    delete ui;
}

void FournisseurMainWindow::on_button_logout_clicked() {
    emit logoutRequested();
}

void FournisseurMainWindow::updateAuthUserLabel() {
    Employe emp;
    ui->label_authUser->setText("Logged in as: " + emp.getAuthenticatedUser());
}

QString FournisseurMainWindow::generateCaptchaText(int length)
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

bool FournisseurMainWindow::verifyCaptcha()
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

void FournisseurMainWindow::on_pb_ajouter_clicked()
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

void FournisseurMainWindow::on_pb_supprimer_clicked()
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

void FournisseurMainWindow::on_pb_modifier_clicked()
{
    int id_fournisseur = ui->le_id_5->text().toInt();
    QString nom_fournisseur = ui->le_nom_fournisseur_3->text();
    QString adresse = ui->le_adresse_3->text();
    QString telephone_fournisseur = ui->le_telephone_3->text();
    QString email_fournisseur = ui->le_email_fournisseur_3->text().trimmed();
    QString type_fournisseur = ui->le_type_fournisseur_3->text().trimmed();

    if (id_fournisseur == 0 || nom_fournisseur.isEmpty() || adresse.isEmpty() ||
        telephone_fournisseur.isEmpty() || email_fournisseur.isEmpty() || type_fournisseur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    bool erreur = false;

    if (type_fournisseur != "stratégiques" && type_fournisseur != "sous-traitants") {
        ui->label_erreur_type_3->setText("Type de fournisseur inexistant, soit stratégiques , sous-traitants.");
        erreur = true;
    } else {
        ui->label_erreur_type_3->clear();
    }

    if (!email_fournisseur.endsWith("@gmail.com")) {
        ui->label_erreur_email_3->setText("L'adresse email doit se terminer par @gmail.com.");
        erreur = true;
    } else {
        ui->label_erreur_email_3->clear();
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

void FournisseurMainWindow::updateStatsChart()
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

void FournisseurMainWindow::on_Sp_Button_ExportPDF_clicked()
{
    f.printPDF();
}

void FournisseurMainWindow::on_pb_id_clicked()
{
    ui->tab_fournisseur->setModel(f.afficherTriParId());
}

void FournisseurMainWindow::on_pb_tri_parnom_clicked()
{
    ui->tab_fournisseur->setModel(f.trierParNom());
}

void FournisseurMainWindow::on_pb_tri_type_clicked()
{
    // TODO: Implement sorting by type
}

void FournisseurMainWindow::on_lineEditRecherche_textChanged(const QString &text)
{
    fournisseur f;
    ui->tab_fournisseur->setModel(f.rechercherParNom(text));
}

void FournisseurMainWindow::on_pb_refresh_stats_clicked()
{
    updateStatsChart();
}

void FournisseurMainWindow::on_pb_importer_clicked()
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

void FournisseurMainWindow::on_btnEmployees_clicked() {
    emit navigateTo(0); // Navigate to Employees
}

void FournisseurMainWindow::on_btnSponsors_clicked() {
    emit navigateTo(1); // Navigate to Sponsors
}

void FournisseurMainWindow::on_btnServices_clicked() {
    emit navigateTo(2); // Navigate to Services
}

void FournisseurMainWindow::on_btnEvents_clicked() {
    emit navigateTo(3); // Navigate to Events
}

void FournisseurMainWindow::on_btnStores_clicked() {
    emit navigateTo(5); // Navigate to Stores
}
