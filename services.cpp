#include "services.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

Services::Services() {}

Services::Services(int id, QString nom, QString type, QDate date, QString etat)
    : ID_Service(id), Nom_Service(nom), Type_Service(type), Date_Service(date), Etat(etat) {}

bool Services::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Services (ID_Service, Nom_Service, Type_Service, Date_Service, Etat) "
                  "VALUES (:ID_Service, :Nom_Service, :Type_Service, :Date_Service, :Etat)");

    query.bindValue(":ID_Service", ID_Service);
    query.bindValue(":Nom_Service", Nom_Service);
    query.bindValue(":Type_Service", Type_Service);
    query.bindValue(":Date_Service", Date_Service.toString("yyyy-MM-dd"));
    query.bindValue(":Etat", Etat);

    if (!query.exec()) {
        qDebug() << "❌ Erreur SQL lors de l'ajout :" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Services::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    if (!query.exec("SELECT ID_Service, Nom_Service, Type_Service, Date_Service, Etat FROM Services")) {
        qDebug() << "Erreur dans afficher() :" << query.lastError().text();
    } else {
        qDebug() << "Requête afficher() exécutée avec succès.";
    }
    model->setQuery(std::move(query));
    qDebug() << "Nombre de lignes dans le modèle :" << model->rowCount();
    return model;
}

bool Services::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Services WHERE ID_Service = :ID_Service");
    query.bindValue(":ID_Service", id);
    return query.exec();
}

bool Services::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE Services SET Nom_Service = :Nom_Service, Type_Service = :Type_Service, Date_Service = :Date_Service, "
                  "Etat = :Etat "
                  "WHERE ID_Service = :ID_Service");

    query.bindValue(":Nom_Service", Nom_Service);
    query.bindValue(":Type_Service", Type_Service);
    query.bindValue(":Date_Service", Date_Service.toString("yyyy-MM-dd"));
    query.bindValue(":Etat", Etat);
    query.bindValue(":ID_Service", id);

    return query.exec();
}

QSqlQueryModel* Services::trierParNom()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("SELECT ID_Service, Nom_Service, Type_Service, Date_Service, Etat FROM Services ORDER BY Nom_Service ASC");
    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* Services::trierParType()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("SELECT ID_Service, Nom_Service, Type_Service, Date_Service, Etat FROM Services ORDER BY Type_Service ASC");
    model->setQuery(std::move(query));
    return model;
}

QSqlQueryModel* Services::rechercherParType(const QString &type)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_Service, Nom_Service, Type_Service, Date_Service, Etat FROM Services WHERE Type_Service LIKE :type");
    query.bindValue(":type", "%" + type + "%");
    query.exec();
    model->setQuery(std::move(query));
    return model;
}
