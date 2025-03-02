#include "sponsor.h"
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>

Sponsor::Sponsor()
{
    idSponsor = 0;
    numtelSponsor = 0;
    nomSponsor = "";
    prenomSponsor = "";
    emailSponsor = "";
    dateDebutSponsor = QDate::currentDate(); // Default to current date
    dateFinSponsor = QDate::currentDate();   // Default to current date
}

Sponsor::Sponsor(int idSponsor, QString nomSponsor, QString prenomSponsor, QString emailSponsor, int numtelSponsor, QDate dateDebutSponsor, QDate dateFinSponsor)
{
    this->idSponsor = idSponsor;
    this->nomSponsor = nomSponsor;
    this->prenomSponsor = prenomSponsor;
    this->emailSponsor = emailSponsor;
    this->numtelSponsor = numtelSponsor;
    this->dateDebutSponsor = dateDebutSponsor;
    this->dateFinSponsor = dateFinSponsor;
}

bool Sponsor::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO SPONSORS (ID_SPONSOR, NOM_SPONSOR, PRENOM_SPONSOR, NUMTEL_SPONSOR,  EMAIL_SPONSOR,DATE_DEBUT_SPONSOR, DATE_FIN_SPONSOR) "
                  "VALUES (:ID_SPONSOR, :NOM_SPONSOR, :PRENOM_SPONSOR,  :NUMTEL_SPONSOR,:EMAIL_SPONSOR, :DATE_DEBUT_SPONSOR, :DATE_FIN_SPONSOR)");
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":PRENOM_SPONSOR", prenomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":NUMTEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT_SPONSOR", dateDebutSponsor);
    query.bindValue(":DATE_FIN_SPONSOR", dateFinSponsor);
    return query.exec();
}

bool Sponsor::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE SPONSORS SET NOM_SPONSOR=:NOM_SPONSOR, PRENOM_SPONSOR=:PRENOM_SPONSOR, NUMTEL_SPONSOR=:NUMTEL_SPONSOR, "
                  "EMAIL_SPONSOR=:EMAIL_SPONSOR, DATE_DEBUT_SPONSOR=:DATE_DEBUT_SPONSOR, DATE_FIN_SPONSOR=:DATE_FIN_SPONSOR "
                  "WHERE ID_SPONSOR=:ID_SPONSOR");
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":PRENOM_SPONSOR", prenomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":NUMTEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT_SPONSOR", dateDebutSponsor);
    query.bindValue(":DATE_FIN_SPONSOR", dateFinSponsor);
    return query.exec();
}

bool Sponsor::supprimer(int idSponsor)
{
    QSqlQuery query;
    query.prepare("DELETE FROM SPONSORS WHERE ID_SPONSOR = :ID_SPONSOR");
    query.bindValue(":ID_SPONSOR", idSponsor);
    return query.exec();
}

QSqlQueryModel* Sponsor::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, PRENOM_SPONSOR, TO_CHAR(NUMTEL_SPONSOR) AS NUMTEL_SPONSOR, "
                    "EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT_SPONSOR, 'YYYY-MM-DD') AS DATE_DEBUT_SPONSOR, "
                    "TO_CHAR(DATE_FIN_SPONSOR, 'YYYY-MM-DD') AS DATE_FIN_SPONSOR FROM SPONSORS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numéro de Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Fin"));
    return model;
}

QSqlQueryModel* Sponsor::afficher_id()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_SPONSOR FROM SPONSORS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    return model;
}
