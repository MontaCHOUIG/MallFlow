#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection {
public:
    Connection();
    bool createconnect();
    void closeConnection(); // Ajout d'une méthode pour fermer la connexion

private:
    QSqlDatabase db; // Déclarer db comme attribut de la classe
};

#endif // CONNECTION_H
