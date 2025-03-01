#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ProjetCPP");
    db.setUserName("Slim");
    db.setPassword("Slim");

    if (db.open())
        test=true;





    return  test;
}
