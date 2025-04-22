#include "main_application.h"
#include "src/employees/employees_mainwindow.h"
#include "src/sponsors/sponsors_mainwindow.h"
#include "src/services/service_mainwindow.h"
#include <QFile>
#include <QDebug>

MainApplication::MainApplication(QWidget *parent) : QMainWindow(parent) {
    // Set up tab widget
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    setWindowTitle("Employee, Sponsor & Service Management");
    resize(1400, 800); // Match employees.ui dimensions

    setupTabs();
    loadStylesheet();
}

void MainApplication::setupTabs() {
    // Embed EmployeesMainWindow
    employeesMainWindow = new EmployeesMainWindow(this);
    tabWidget->addTab(employeesMainWindow, "Employees");

    // Embed SponsorsMainWindow
    sponsorsMainWindow = new SponsorsMainWindow(this);
    tabWidget->addTab(sponsorsMainWindow, "Sponsors");

    // Embed ServicesMainWindow
    servicesMainWindow = new ServiceMainWindow(this);
    tabWidget->addTab(servicesMainWindow, "Services");
}

void MainApplication::loadStylesheet() {
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        qDebug() << "Failed to load stylesheet";
    }
}

MainApplication::~MainApplication() {
    delete employeesMainWindow;
    delete sponsorsMainWindow;
    delete servicesMainWindow;
}
