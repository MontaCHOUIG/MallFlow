#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MallFlow");
    ui->Sp_TableView_Res->setModel(S.afficher());
    ui->Sp_TableView_Res->setColumnWidth(3, 250);
    ui->Sp_Combo_IDs->setModel(S.afficher_id());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Sp_Button_Ajouter_clicked()
{
    int idSponsor = ui->Sp_Line_ID->text().toInt(); // Changed from line_ID to Sp_Line_ID
    QString nomSponsor = ui->Sp_Line_Nom->text(); // Changed from line_nom to Sp_Line_Nom
    QString prenomSponsor = ui->Sp_Line_Prenom->text(); // Changed from line_prenom to Sp_Line_Prenom
    QString emailSponsor = ui->Sp_Line_Email->text(); // Changed from line_email to Sp_Line_Email
    int numtelSponsor = ui->Sp_Line_Numtel->text().toInt(); // Changed from line_numtel to Sp_Line_Numtel
    QString idSponsorString = ui->Sp_Line_ID->text(); // Changed from line_ID to Sp_Line_ID
    QString numtelSponsorString = ui->Sp_Line_Numtel->text(); // Changed from line_numtel to Sp_Line_Numtel
    QDate dateDebutSponsor = ui->dateEdit_Debut->date(); // Added date from UI
    QDate dateFinSponsor = ui->dateEdit_Fin->date(); // Added date from UI

    if (numtelSponsorString.isEmpty() || emailSponsor.isEmpty() || idSponsorString.isEmpty() || idSponsor == 0 ||
        nomSponsor.isEmpty() || prenomSponsor.isEmpty()) {
        ui->Sp_Label_InfoAffichage->setText("Erreur de controle de saisire"); // Changed from label_info_gestion to Sp_Label_InfoAffichage
        return;
    }

    Sponsor S(idSponsor, nomSponsor, prenomSponsor, emailSponsor, numtelSponsor, dateDebutSponsor, dateFinSponsor);
    ui->Sp_Label_InfoAffichage->setText("Ajout Effectué ID: " + QString::number(idSponsor)); // Changed CIN to idSponsor
    bool test = S.ajouter();
    if (test) {
        ui->Sp_TableView_Res->setModel(S.afficher()); // Changed table_Clients to Sp_TableView_Res
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();// Changed comboBox_IDs to Sp_Combo_IDs
    } else {
        ui->Sp_Label_InfoAffichage->setText("non effectué"); // Changed label_info_gestion to Sp_Label_InfoAffichage
    }
}

void MainWindow::on_Sp_Button_Modifier_clicked()
{
    int idSponsor = ui->Sp_Line_ID->text().toInt(); // Changed from line_ID to Sp_Line_ID
    QString nomSponsor = ui->Sp_Line_Nom->text(); // Changed from line_nom to Sp_Line_Nom
    QString prenomSponsor = ui->Sp_Line_Prenom->text(); // Changed from line_prenom to Sp_Line_Prenom
    QString emailSponsor = ui->Sp_Line_Email->text(); // Changed from line_email to Sp_Line_Email
    int numtelSponsor = ui->Sp_Line_Numtel->text().toInt(); // Changed from line_numtel to Sp_Line_Numtel
    QString idSponsorString = ui->Sp_Line_ID->text(); // Changed from line_ID to Sp_Line_ID
    QString numtelSponsorString = ui->Sp_Line_Numtel->text(); // Changed from line_numtel to Sp_Line_Numtel
    QDate dateDebutSponsor = ui->dateEdit_Debut->date(); // Added date from UI
    QDate dateFinSponsor = ui->dateEdit_Fin->date(); // Added date from UI

    if (numtelSponsorString.isEmpty() || emailSponsor.isEmpty() || idSponsorString.isEmpty() || idSponsor == 0 ||
        nomSponsor.isEmpty() || prenomSponsor.isEmpty()) {
        ui->Sp_Label_InfoAffichage->setText("Erreur de controle de saisire"); // Changed from label_info_gestion to Sp_Label_InfoAffichage
        return;
    }

    Sponsor S(idSponsor, nomSponsor, prenomSponsor, emailSponsor, numtelSponsor, dateDebutSponsor, dateFinSponsor);
    bool test = S.modifier();
    if (test) {
        ui->Sp_Label_InfoAffichage->setText("Modification Effectué ID: " + QString::number(idSponsor)); // Changed CIN to idSponsor
        ui->Sp_TableView_Res->setModel(S.afficher()); // Changed table_Clients to Sp_TableView_Res
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();// Changed comboBox_IDs to Sp_Combo_IDs
    } else {
        ui->Sp_Label_InfoAffichage->setText("Modification non effectué"); // Fixed typo and changed label
    }
}

void MainWindow::on_Sp_Button_Supprimer_clicked()
{
    Sponsor S;
    S.setIdSponsor(ui->Sp_Combo_IDs->currentText().toInt()); // Changed comboBox_IDs to Sp_Combo_IDs
    bool test = S.supprimer(S.getIdSponsor());
    if (test) {
        ui->Sp_Label_InfoAffichage->setText("Suppression Effectué"); // Changed label_info_gestion to Sp_Label_InfoAffichage
        ui->Sp_TableView_Res->setModel(S.afficher()); // Changed table_Clients to Sp_TableView_Res
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();// Changed comboBox_IDs to Sp_Combo_IDs
    } else {
        ui->Sp_Label_InfoAffichage->setText("Suppression non effectué"); // Changed label_info_gestion to Sp_Label_InfoAffichage
    }
}

void MainWindow::on_Sp_Combo_IDs_currentIndexChanged(int index)
{
    int idSponsor = ui->Sp_Combo_IDs->currentText().toInt(); // Changed comboBox_IDs to Sp_Combo_IDs
    QString idSponsorString = QString::number(idSponsor);
    QSqlQuery query;
    query.prepare("SELECT * FROM SPONSORS WHERE ID_SPONSOR = :ID_SPONSOR"); // Changed CLIENT to SPONSORS and used parameter binding
    query.bindValue(":ID_SPONSOR", idSponsor);
    if (query.exec()) {
        while (query.next()) {
            ui->Sp_Line_ID->setText(query.value(0).toString()); // Changed line_ID to Sp_Line_ID
            ui->Sp_Line_Nom->setText(query.value(1).toString()); // Changed line_nom to Sp_Line_Nom
            ui->Sp_Line_Prenom->setText(query.value(2).toString()); // Changed line_prenom to Sp_Line_Prenom
            ui->Sp_Line_Email->setText(query.value(3).toString()); // Changed line_email to Sp_Line_Email
            ui->Sp_Line_Numtel->setText(query.value(4).toString()); // Changed line_numtel to Sp_Line_Numtel
            ui->dateEdit_Debut->setDate(query.value(5).toDate()); // Added date handling
            ui->dateEdit_Fin->setDate(query.value(6).toDate()); // Added date handling
        }
    } else {
        ui->Sp_Label_InfoAffichage->setText("Echec de chargement"); // Changed label_info_gestion to Sp_Label_InfoAffichage
    }
}
void MainWindow::clearFields()
{
    // Clear all QLineEdit fields
    ui->Sp_Line_ID->clear();
    ui->Sp_Line_Nom->clear();
    ui->Sp_Line_Prenom->clear();
    ui->Sp_Line_Email->clear();
    ui->Sp_Line_Numtel->clear();

    // Reset QDateEdit fields to a default date (e.g., current date or a specific date)
    ui->dateEdit_Debut->setDate(QDate::currentDate()); // Or set to a specific default, e.g., QDate(2000, 1, 1)
    ui->dateEdit_Fin->setDate(QDate::currentDate());   // Or set to a specific default
}




