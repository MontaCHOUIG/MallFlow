#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fournisseur.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlError>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pb_id, &QPushButton::clicked, this, &MainWindow::on_pb_id_clicked);

    ui->le_id->setValidator(new QIntValidator(0, 999999, this));
    ui->tab_fournisseur->setModel(f.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_ajouter_clicked()
{
    int id_fournisseur = ui->le_id->text().toInt();
    QString nom_fournisseur = ui->le_nom_fournisseur->text();
    QString adresse = ui->le_adresse->text();
    QString telephone_fournisseur = ui->le_telephone_fournisseur->text();
    QString email_fournisseur = ui->le_email_fournisseur->text();
    QString date_contrat = ui->le_date_contrat->text();

    // Vérification si la date est vide
    if (date_contrat.isEmpty()) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("La date de contrat ne peut pas être vide."), QMessageBox::Cancel);
        return;
    }

    fournisseur f(id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, date_contrat);
    bool test = f.ajouter();
    if(test){
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
    QString email_fournisseur = ui->le_email_fournisseur_3->text();
    QString date_contrat = ui->le_date_contrat_3->text();

    if (id_fournisseur == 0 || nom_fournisseur.isEmpty() || adresse.isEmpty() ||
        telephone_fournisseur.isEmpty() || email_fournisseur.isEmpty() || date_contrat.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
        return;
    }

    if (f.modifier(id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, date_contrat)) {
        QMessageBox::information(this, "Succès", "Modification réussie.");
        ui->tab_fournisseur->setModel(f.afficher()); // Actualiser l'affichage
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
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


void MainWindow::on_pb_tri_contrat_clicked()
{

}

