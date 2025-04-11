#ifndef SPONSOR_H
#define SPONSOR_H
#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>

class Sponsor
{
    int idSponsor;
    int numtelSponsor;
    QString nomSponsor;
    QString emailSponsor;
    QDate dateDebutSponsor;
    QDate dateFinSponsor;

public:
    Sponsor();
    Sponsor(int, QString, QString, int, QDate, QDate);

    int getIdSponsor() { return idSponsor; }
    int getNumtelSponsor() { return numtelSponsor; }
    QString getNomSponsor() { return nomSponsor; }
    QString getEmailSponsor() { return emailSponsor; }
    QDate getDateDebutSponsor() { return dateDebutSponsor; }
    QDate getDateFinSponsor() { return dateFinSponsor; }

    void setIdSponsor(int idSponsor) { this->idSponsor = idSponsor; }
    void setNumtelSponsor(int numtelSponsor) { this->numtelSponsor = numtelSponsor; }
    void setNomSponsor(QString nomSponsor) { this->nomSponsor = nomSponsor; }
    void setEmailSponsor(QString emailSponsor) { this->emailSponsor = emailSponsor; }
    void setDateDebutSponsor(QDate dateDebutSponsor) { this->dateDebutSponsor = dateDebutSponsor; }
    void setDateFinSponsor(QDate dateFinSponsor) { this->dateFinSponsor = dateFinSponsor; }
    QSqlQueryModel* Afficher_Tri_NOM();
    QSqlQueryModel* Afficher_Tri_EMAIL();
    QSqlQueryModel* Afficher_Tri_TEL();
    void Recherche(QTableView *table, QString x);
    void clearTable(QTableView *table);
    void postrequest(QString smsmsg,QString phonenumber);

    bool ajouter();
    bool supprimer(int idSponsor);
    bool modifier();
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficher_id();
};

#endif
