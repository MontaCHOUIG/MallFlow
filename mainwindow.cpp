#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "services.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion des boutons aux slots
    connect(ui->Sp_Button_Ajouter, SIGNAL(clicked()), this, SLOT(onBtnAjouterClicked()));
    connect(ui->Sp_Button_Modifier_3, SIGNAL(clicked()), this, SLOT(onBtnModifierClicked()));
    connect(ui->Sp_Button_Modifier_4, SIGNAL(clicked()), this, SLOT(onBtnSupprimerClicked()));
    connect(ui->Sp_Button_Tri_Nom, SIGNAL(clicked()), this, SLOT(onBtnTriNomClicked()));
    connect(ui->Sp_Button_Tri_Type, SIGNAL(clicked()), this, SLOT(onBtnTriTypeClicked()));
    connect(ui->Sp_Line_Recherche, SIGNAL(textChanged(QString)), this, SLOT(onRechercheTextChanged(QString)));

    // Charger les données au démarrage
    onBtnAfficherClicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ➤ Récupérer l'ID du service sélectionné
int MainWindow::getSelectedServiceID()
{
    // Récupérer l'index de la ligne sélectionnée
    QModelIndexList selectedIndexes = ui->Sp_TableView_Res->selectionModel()->selectedRows();

    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un service.");
        return -1; // Retourne -1 si aucun élément n'est sélectionné
    }

    // Récupérer l'ID de la première ligne sélectionnée
    int row = selectedIndexes.at(0).row();
    int id = ui->Sp_TableView_Res->model()->index(row, 0).data().toInt(); // Colonne 0 = ID_Service

    return id;
}

// ➤ Ajouter un service
void MainWindow::onBtnAjouterClicked()
{
    int id = ui->Sp_Line_ID_2->text().toInt();
    QString nom = ui->Sp_Line_Nom_2->text();
    QString type = ui->comboBox->currentText();
    QDate date = ui->dateEdit->date();
    QString etat = ui->Sp_Line_Email_2->text();

    Services s(id, nom, type, date, etat);
    if (s.ajouter()) {
        qDebug() << "Ajout réussi !";
        QMessageBox::information(this, "Succès", "Service ajouté avec succès.");
        onBtnAfficherClicked(); // Mettre à jour l'affichage
    } else {
        qDebug() << "Échec de l'ajout.";
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout du service.");
    }
}

// ➤ Afficher les services
void MainWindow::onBtnAfficherClicked()
{
    Services s;
    ui->tabWidget->setCurrentIndex(0); // Sélectionne l'onglet contenant la table
    ui->Sp_TableView_Res->setModel(s.afficher());
}

// ➤ Supprimer un service
void MainWindow::onBtnSupprimerClicked()
{
    int id = getSelectedServiceID(); // Récupérer l'ID du service sélectionné
    if (id == -1) return; // Si aucun service n'est sélectionné, on ne fait rien

    Services s;
    if (s.supprimer(id)) {
        qDebug() << "Suppression réussie !";
        QMessageBox::information(this, "Succès", "Service supprimé avec succès.");
        onBtnAfficherClicked(); // Mettre à jour l'affichage
    } else {
        qDebug() << "Échec de la suppression.";
        QMessageBox::warning(this, "Erreur", "Échec de la suppression du service.");
    }
}

// ➤ Modifier un service
void MainWindow::onBtnModifierClicked()
{
    int id = ui->Sp_Line_ID_2->text().toInt(); // Récupérer l'ID du service à modifier
    if (id <= 0) {
        QMessageBox::warning(this, "Avertissement", "Veuillez entrer un ID valide.");
        return;
    }

    // Récupérer les nouvelles valeurs des champs
    QString nom = ui->Sp_Line_Nom_2->text();
    QString type = ui->comboBox->currentText();
    QDate date = ui->dateEdit->date();
    QString etat = ui->Sp_Line_Email_2->text();

    Services s(id, nom, type, date, etat);
    if (s.modifier(id)) {
        qDebug() << "Modification réussie !";
        QMessageBox::information(this, "Succès", "Service modifié avec succès.");
        onBtnAfficherClicked(); // Mettre à jour l'affichage
    } else {
        qDebug() << "Échec de la modification.";
        QMessageBox::warning(this, "Erreur", "Échec de la modification du service.");
    }
}

// ➤ Tri par nom
void MainWindow::onBtnTriNomClicked()
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.trierParNom());
}

// ➤ Tri par type
void MainWindow::onBtnTriTypeClicked()
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.trierParType());
}

// ➤ Recherche par type
void MainWindow::onRechercheTextChanged(const QString &text)
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.rechercherParType(text));
}
