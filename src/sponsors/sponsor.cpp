#include "sponsor.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>

// Constructeur par défaut de la classe Sponsor
Sponsor::Sponsor()
{
    // Initialisation des attributs avec des valeurs par défaut
    idSponsor = 0; // ID du sponsor à 0
    numtelSponsor = 0; // Numéro de téléphone à 0
    nomSponsor = ""; // Nom vide
    emailSponsor = ""; // Email vide
    dateDebutSponsor = QDate::currentDate(); // Date de début actuelle
    dateFinSponsor = QDate::currentDate();   // Date de fin actuelle
}

// Constructeur avec paramètres pour initialiser les attributs du sponsor
Sponsor::Sponsor(int idSponsor, QString nomSponsor, QString emailSponsor, int numtelSponsor, QDate dateDebutSponsor, QDate dateFinSponsor)
{
    // Assignation des valeurs des paramètres aux attributs de l'objet
    this->idSponsor = idSponsor;
    this->nomSponsor = nomSponsor;
    this->emailSponsor = emailSponsor;
    this->numtelSponsor = numtelSponsor;
    this->dateDebutSponsor = dateDebutSponsor;
    this->dateFinSponsor = dateFinSponsor;
}

// Méthode pour ajouter un sponsor dans la base de données
bool Sponsor::ajouter()
{
    // Création d'une requête SQL préparée pour éviter les injections SQL
    QSqlQuery query;
    query.prepare("INSERT INTO SPONSORS (ID_SPONSOR, NOM_SPONSOR, TEL_SPONSOR, EMAIL_SPONSOR, DATE_DEBUT, DATE_FIN) "
                  "VALUES (:ID_SPONSOR, :NOM_SPONSOR, :TEL_SPONSOR, :EMAIL_SPONSOR, :DATE_DEBUT, :DATE_FIN)");

    // Liaison des valeurs des attributs de l'objet avec les paramètres de la requête SQL
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":TEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT", dateDebutSponsor);
    query.bindValue(":DATE_FIN", dateFinSponsor);

    // Exécution de la requête SQL, renvoie true si réussie, false sinon
    return query.exec();
}

// Méthode pour modifier un sponsor existant dans la base de données
bool Sponsor::modifier()
{
    // Préparation d'une requête SQL pour mettre à jour un sponsor en fonction de son ID
    QSqlQuery query;
    query.prepare("UPDATE SPONSORS SET NOM_SPONSOR=:NOM_SPONSOR, TEL_SPONSOR=:TEL_SPONSOR, "
                  "EMAIL_SPONSOR=:EMAIL_SPONSOR, DATE_DEBUT=:DATE_DEBUT, DATE_FIN=:DATE_FIN "
                  "WHERE ID_SPONSOR=:ID_SPONSOR");

    // Liaison des valeurs des attributs avec les paramètres de la requête
    query.bindValue(":ID_SPONSOR", idSponsor);
    query.bindValue(":NOM_SPONSOR", nomSponsor);
    query.bindValue(":EMAIL_SPONSOR", emailSponsor);
    query.bindValue(":TEL_SPONSOR", numtelSponsor);
    query.bindValue(":DATE_DEBUT", dateDebutSponsor);
    query.bindValue(":DATE_FIN", dateFinSponsor);

    // Exécution de la requête SQL pour modifier, renvoie true si réussite, false sinon
    return query.exec();
}

// Méthode pour supprimer un sponsor à partir de son ID
bool Sponsor::supprimer(int idSponsor)
{
    // Préparation de la requête SQL pour supprimer un sponsor de la base de données
    QSqlQuery query;
    query.prepare("DELETE FROM SPONSORS WHERE ID_SPONSOR = :ID_SPONSOR");

    // Liaison de l'ID du sponsor avec le paramètre de la requête
    query.bindValue(":ID_SPONSOR", idSponsor);

    // Exécution de la requête SQL pour supprimer, renvoie true si réussie, false sinon
    return query.exec();
}

// Méthode pour afficher tous les sponsors dans un modèle de données (QSqlQueryModel)
QSqlQueryModel* Sponsor::afficher()
{
    // Création d'un modèle pour afficher les données des sponsors
    QSqlQueryModel* model = new QSqlQueryModel();

    // Exécution de la requête SQL pour sélectionner les sponsors et formater les dates
    model->setQuery("SELECT TO_CHAR(ID_SPONSOR) AS ID_SPONSOR, NOM_SPONSOR, TO_CHAR(TEL_SPONSOR) AS TEL_SPONSOR, "
                    "EMAIL_SPONSOR, TO_CHAR(DATE_DEBUT, 'YYYY-MM-DD') AS DATE_DEBUT, "
                    "TO_CHAR(DATE_FIN, 'YYYY-MM-DD') AS DATE_FIN FROM SPONSORS");

    // Définition des en-têtes de colonnes du tableau de résultats
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Numéro de Téléphone"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Fin"));

    // Renvoie du modèle contenant les données des sponsors
    return model;
}

// Méthode pour afficher uniquement les ID des sponsors
QSqlQueryModel* Sponsor::afficher_id()
{
    // Création d'un modèle pour afficher les ID des sponsors
    QSqlQueryModel* model = new QSqlQueryModel();

    // Exécution de la requête SQL pour sélectionner uniquement les ID des sponsors
    model->setQuery("SELECT ID_SPONSOR FROM SPONSORS");

    // Définition de l'en-tête de colonne du tableau de résultats
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Sponsor"));

    // Renvoie du modèle contenant les ID des sponsors
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

void Sponsor::postrequest(QString smsmsg, QString phonenumber) {
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // Replace with your Twilio credentials and phone number
    QString accountSid = "AC6e26f6db1c043fe90779d6b644b7cd71";
    QString authToken = "4ae04e06baed68e4ca48d1339476d6e9";
    QString fromNumber = "+18053603109"; // Your Twilio phone number

    QUrl url(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSid));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Set up Basic Auth header
    QString auth = "Basic " + QString("%1:%2").arg(accountSid, authToken).toUtf8().toBase64();
    req.setRawHeader("Authorization", auth.toUtf8());

    // Form the data as application/x-www-form-urlencoded
    QUrlQuery params;
    params.addQueryItem("To", "+216" + phonenumber);
    params.addQueryItem("From", fromNumber);
    params.addQueryItem("Body", smsmsg);

    QByteArray data = params.query().toUtf8();
    QNetworkReply *reply = mgr.post(req, data);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success" << reply->readAll();
        delete reply;
    } else {
        qDebug() << "Failure" << reply->errorString() << reply->error();
        delete reply;
    }
}
