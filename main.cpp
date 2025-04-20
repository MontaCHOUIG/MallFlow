#include "main_application.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Connexion à la base de données
    Connection c;
    bool test = c.createconnect();

    if (!test) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la connexion à la base de données.");
        return -1; // Quitter l'application si la connexion échoue
    }

    MainApplication w;
    w.show();
    return a.exec();
}
