#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>  // Ajout pour debug

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Début du programme";

    Connection c;
    bool test = c.createconnect();

    MainWindow w; // Déclaration après la connexion

    if (test)
    {
        qDebug() << "Connexion réussie, affichage de la fenêtre.";
        w.show();
        QMessageBox::information(nullptr, "Database", "Connexion réussie.");
    }
    else
    {
        qDebug() << "Échec de la connexion.";
        QMessageBox::critical(nullptr, "Database", "Connexion échouée.");
    }

    return a.exec();
}
