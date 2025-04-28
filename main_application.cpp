#include "main_application.h"
#include <QFile>
#include <QDebug>

MainApplication::MainApplication(QWidget *parent) : QMainWindow(parent) {
    // Set up tab widget
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
    setWindowTitle("MallFlow Management");
    resize(1400, 800);

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

    // Embed EvenementMainWindow
    evenementMainWindow = new EvenementMainWindow(this);
    tabWidget->addTab(evenementMainWindow, "Events");

    // Embed FournisseurMainWindow
    fournisseurMainWindow = new FournisseurMainWindow(this);
    tabWidget->addTab(fournisseurMainWindow, "Suppliers");

    // Embed MagasinMainWindow
    magasinMainWindow = new MagasinMainWindow(this);
    tabWidget->addTab(magasinMainWindow, "Stores");
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
    delete evenementMainWindow;
    delete fournisseurMainWindow;
    delete magasinMainWindow;
}
