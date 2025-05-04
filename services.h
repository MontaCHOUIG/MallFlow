#ifndef SERVICES_H
#define SERVICES_H

#include <QDate>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Services
{
public:
    Services();
    Services(int id, QString nom, QString type, QDate date, QString etat);

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id);
    bool modifier(int id);
    QSqlQueryModel * trierParNom();
    QSqlQueryModel * trierParType();
    QSqlQueryModel * rechercherParType(const QString &type);

private:
    int ID_Service;
    QString Nom_Service;
    QString Type_Service;
    QDate Date_Service;
    QString Etat;
};

#endif // SERVICES_H
