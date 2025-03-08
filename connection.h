#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class  Connection
{
    QSqlDatabase db;
public:
    Connection();
    bool CreateConnexion();
    bool OuvrirConnexion();
    void FermerConnexion();
};

#endif // CONNEXION_H
