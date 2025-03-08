#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection() {

    db = QSqlDatabase::addDatabase("QODBC"); // Initialize the database connection only once
    db.setDatabaseName("ProjetCPP");
    db.setUserName("Slim");
    db.setPassword("Slim");
}

bool Connection::CreateConnexion() {
    bool test = false;
    if (db.open()) {
        test = true;
    } else {
        qDebug() << "Database Error: " << db.lastError().text();
    }
    return test;
}

void Connection::FermerConnexion() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Connection::OuvrirConnexion() {
    return db.open();
}
