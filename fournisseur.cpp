#include "fournisseur.h"
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>


fournisseur::fournisseur()
{
    id_fournisseur = 0;
    nom_fournisseur = " ";
    adresse = " ";
    telephone_fournisseur = " ";
    email_fournisseur = " ";
    date_contrat = " ";
}

fournisseur::fournisseur(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString date_contrat)
{
    this->id_fournisseur = id_fournisseur;
    this->nom_fournisseur = nom_fournisseur;
    this->adresse = adresse;
    this->telephone_fournisseur = telephone_fournisseur;
    this->email_fournisseur = email_fournisseur;
    this->date_contrat = date_contrat;
}

int fournisseur::getIdFournisseur() { return id_fournisseur; }
QString fournisseur::getNomFournisseur() { return nom_fournisseur; }
QString fournisseur::getAdresse() { return adresse; }
QString fournisseur::getTelephoneFournisseur() { return telephone_fournisseur; }
QString fournisseur::getEmailFournisseur() { return email_fournisseur; }
QString fournisseur::getDateContrat() { return date_contrat; }

void fournisseur::setIdFournisseur(int id_fournisseur) { this->id_fournisseur = id_fournisseur; }
void fournisseur::setNomFournisseur(QString nom_fournisseur) { this->nom_fournisseur = nom_fournisseur; }
void fournisseur::setAdresse(QString adresse) { this->adresse = adresse; }
void fournisseur::setTelephoneFournisseur(QString telephone_fournisseur) { this->telephone_fournisseur = telephone_fournisseur; }
void fournisseur::setEmailFournisseur(QString email_fournisseur) { this->email_fournisseur = email_fournisseur; }
void fournisseur::setDateContrat(QString date_contrat) { this->date_contrat = date_contrat; }

bool fournisseur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO FOURNISSEURS (id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, date_contrat) "
                  "VALUES (:id_fournisseur, :nom_fournisseur, :adresse, :telephone_fournisseur, :email_fournisseur, :date_contrat)");
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":nom_fournisseur", nom_fournisseur);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone_fournisseur", telephone_fournisseur);
    query.bindValue(":email_fournisseur", email_fournisseur);
    query.bindValue(":date_contrat", date_contrat);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}



QSqlQueryModel* fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS");
    return model;
}

bool fournisseur::supprimer(int id_fournisseur)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FOURNISSEURS WHERE id_fournisseur = :id_fournisseur");
    query.bindValue(":id_fournisseur", id_fournisseur);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;

}


bool fournisseur::modifier(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString date_contrat)
{
    QSqlQuery query;
    query.prepare("UPDATE FOURNISSEURS SET nom_fournisseur=:nom_fournisseur, adresse=:adresse, telephone_fournisseur=:telephone_fournisseur, email_fournisseur=:email_fournisseur, date_contrat=:date_contrat WHERE id_fournisseur=:id_fournisseur");
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":nom_fournisseur", nom_fournisseur);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone_fournisseur", telephone_fournisseur);
    query.bindValue(":email_fournisseur", email_fournisseur);
    query.bindValue(":date_contrat", date_contrat);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

void fournisseur::printPDF()
{
    QString filePath = QFileDialog::getSaveFileName(
        nullptr,
        "Enregistrer le PDF",
        "",
        "Fichiers PDF (*.pdf)"
        );

    if (filePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Annulé", "Aucun emplacement sélectionné.");
        return; // L'utilisateur a annulé
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdf(filePath);
    QPainter painter(&pdf);
    int i = 4000;

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(2000, 1500, "Liste des fournisseurs");

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 15));
    painter.drawRect(100, 100, 8500, 2500); // Encadré
    painter.drawRect(0, 3000, 9600, 500);   // En-tête de tableau

    // En-têtes des colonnes
    painter.setFont(QFont("Arial", 10));
    painter.drawText(200, 3300, "ID");
    painter.drawText(800, 3300, "Nom");
    painter.drawText(2000, 3300, "Adresse");
    painter.drawText(3400, 3300, "Téléphone");
    painter.drawText(5000, 3300, "Email");
    painter.drawText(7000, 3300, "Date contrat");

    QSqlQuery query;
    query.prepare("SELECT * FROM FOURNISSEURS");
    query.exec();

    while (query.next())
    {
        painter.drawText(200, i, query.value(0).toString());      // ID
        painter.drawText(800, i, query.value(1).toString());      // Nom
        painter.drawText(2000, i, query.value(2).toString());     // Adresse
        painter.drawText(3400, i, query.value(3).toString());     // Téléphone
        painter.drawText(5000, i, query.value(4).toString());     // Email
        painter.drawText(7000, i, query.value(5).toString());     // Date contrat
        i += 500;
    }

    QMessageBox::information(nullptr, "PDF généré", "Le fichier PDF a été créé avec succès.");
}

QSqlQueryModel* fournisseur::afficherTriParId()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS ORDER BY id_fournisseur ASC");
    return model;
}
QSqlQueryModel* fournisseur::trierParNom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS ORDER BY nom_fournisseur ASC");
    return model;
}

