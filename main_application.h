#ifndef MAIN_APPLICATION_H
#define MAIN_APPLICATION_H

#include <QMainWindow>
#include <QTabWidget>

class EmployeesMainWindow;
class SponsorsMainWindow;
class ServiceMainWindow;

class MainApplication : public QMainWindow {
    Q_OBJECT
public:
    MainApplication(QWidget *parent = nullptr);
    ~MainApplication();
private:
    QTabWidget *tabWidget;
    EmployeesMainWindow *employeesMainWindow;
    SponsorsMainWindow *sponsorsMainWindow;
    ServiceMainWindow *servicesMainWindow;
    void setupTabs();
    void loadStylesheet();
};

#endif
