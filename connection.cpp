#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    db.setUserName("shopdevs");//inserer nom de l'utilisateur
    db.setPassword("mallflow");//inserer mot de passe de cet utilisateur

    if (db.open())
        test=true;





    return  test;
}

void Connection::closeConnection(){db.close();}
