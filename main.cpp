#include "main_application.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "src/employees/login.h"

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

    // Show login screen first
    Login login;
    Employe e ;
    if (e.getAuthenticatedUser().isEmpty()) {  // Check if a user is already logged in
        if (login.exec() != QDialog::Accepted) {
            return 0;  // Exit if login fails
        }
    }


    MainApplication w;
    w.show();
    return a.exec();
}
