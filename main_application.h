#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <QMainWindow>
#include <QStackedWidget>
#include "src/employees/employees_mainwindow.h"
#include "src/sponsors/sponsors_mainwindow.h"
#include "src/services/service_mainwindow.h"
#include "src/evenements/evenement_mainwindow.h"
#include "src/fournisseurs/fournisseur_mainwindow.h"
#include "src/magasins/magasin_mainwindow.h"

class MainApplication : public QMainWindow {
    Q_OBJECT

public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();

private slots:
    void navigateTo(int index);

private:
    QStackedWidget *stackedWidget;
    EmployeesMainWindow *employeesMainWindow;
    SponsorsMainWindow *sponsorsMainWindow;
    ServiceMainWindow *servicesMainWindow;
    EvenementMainWindow *evenementMainWindow;
    FournisseurMainWindow *fournisseurMainWindow;
    MagasinMainWindow *magasinMainWindow;

    void setupWindows();
    void loadStylesheet();
};

#endif // MAIN_APPLICATION_H
