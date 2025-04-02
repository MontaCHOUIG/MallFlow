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
    emailSponsor = "";
    dateDebutSponsor = QDate::currentDate(); // Default to current date
    dateFinSponsor = QDate::currentDate();   // Default to current date
}

Sponsor::Sponsor(int idSponsor, QString nomSponsor, QString emailSponsor, int numtelSponsor, QDate dateDebutSponsor, QDate dateFinSponsor)
{
    this->idSponsor = idSponsor;
    this->nomSponsor = nomSponsor;
    this->emailSponsor = emailSponsor;
    this->numtelSponsor = numtelSponsor;
    this->dateDebutSponsor = dateDebutSponsor;
    this->dateFinSponsor = dateFinSponsor;
}

bool Sponsor::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO SPONSORS (ID_SPONSOR, NOM_SPONSOR, TEL_SPONSOR,  EMAIL_SPONSOR,DATE_DEBUT, DATE_FIN) "
                  "VALUES (:ID_SPONSOR, :NOM_SPONSOR, :TEL_SPONSOR,:EMAIL_SPONSOR, :DATE_DEBUT, :DATE_FIN)");
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":TEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT", dateDebutSponsor);
    query.bindValue(":DATE_FIN", dateFinSponsor);
    return query.exec();
}

bool Sponsor::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE SPONSORS SET NOM_SPONSOR=:NOM_SPONSOR, TEL_SPONSOR=:TEL_SPONSOR, "
                  "EMAIL_SPONSOR=:EMAIL_SPONSOR, DATE_DEBUT=:DATE_DEBUT, DATE_FIN=:DATE_FIN "
                  "WHERE ID_SPONSOR=:ID_SPONSOR");
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":TEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT", dateDebutSponsor);
    query.bindValue(":DATE_FIN", dateFinSponsor);
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
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, "
                    "EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, "
                    "TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Numéro de Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));
    return model;
}

QSqlQueryModel* Sponsor::afficher_id()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_SPONSOR FROM SPONSORS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    return model;
}
QSqlQueryModel* Sponsor::Afficher_Tri_NOM() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS ORDER BY NOM_SPONSOR");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));

    return model;
}

QSqlQueryModel* Sponsor::Afficher_Tri_EMAIL() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS ORDER BY EMAIL_SPONSOR");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));

    return model;
}

QSqlQueryModel* Sponsor::Afficher_Tri_TEL() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS ORDER BY TEL_SPONSOR");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));

    return model;
}

void Sponsor::Recherche(QTableView *table, QString x) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery;

    if (!x.isEmpty()) {
        query->prepare("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, "
                       "EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, "
                       "TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN "
                       "FROM SPONSORS WHERE regexp_like(NOM_SPONSOR, :X, 'i')");
        query->bindValue(":X", x);
    } else {
        query->prepare("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, "
                       "EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, "
                       "TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS");
    }

    query->exec();
    model->setQuery(*query);

    // 🟢 Set headers just like afficher()
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Numéro de Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));

    table->setModel(model);
    table->show();
}

void Sponsor::clearTable(QTableView *table) {
    QSqlQueryModel* modelFeragh = new QSqlQueryModel();
    modelFeragh->clear();
    table->setModel(modelFeragh);
}


