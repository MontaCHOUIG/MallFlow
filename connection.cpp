#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A"); // Nom de la source de données
    db.setUserName("shopdevs"); // Nom d'utilisateur
    db.setPassword("mallflow"); // Mot de passe

    if (db.open()) {
        qDebug() << "Connexion à la base de données réussie.";
        return true;
    } else {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
        return false;
    }
}

void Connection::closeConnection() {
    db.close();
    qDebug() << "Connexion à la base de données fermée.";
}
