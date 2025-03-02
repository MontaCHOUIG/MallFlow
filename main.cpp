#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;

    // Try to connect to the database first
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Connection failed.\nClick OK to exit."),
                              QMessageBox::Ok);
        return -1;  // Exit if database connection fails
    }

    QMessageBox::information(nullptr, QObject::tr("Database is open"),
                             QObject::tr("Connection successful.\nClick OK to continue."),
                             QMessageBox::Ok);

    // Show Main Window after a successful database connection
    MainWindow w;
    w.show();

    return a.exec();
}
