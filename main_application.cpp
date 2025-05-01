#include "main_application.h"
#include <QFile>
#include <QDebug>

MainApplication::MainApplication(QWidget *parent) : QMainWindow(parent) {
    // Set up stacked widget
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    setWindowTitle("MallFlow Management");
    resize(1400, 800);

    setupWindows();
    loadStylesheet();

    // Connecter les signaux de navigation de chaque fenêtre
    connect(employeesMainWindow, &EmployeesMainWindow::navigateTo, this, &MainApplication::navigateTo);
    connect(sponsorsMainWindow, &SponsorsMainWindow::navigateTo, this, &MainApplication::navigateTo);
    connect(servicesMainWindow, &ServiceMainWindow::navigateTo, this, &MainApplication::navigateTo);
    connect(evenementMainWindow, &EvenementMainWindow::navigateTo, this, &MainApplication::navigateTo);
    connect(fournisseurMainWindow, &FournisseurMainWindow::navigateTo, this, &MainApplication::navigateTo);
    connect(magasinMainWindow, &MagasinMainWindow::navigateTo, this, &MainApplication::navigateTo);
}

void MainApplication::setupWindows() {
    // Create and add all windows to stacked widget
    employeesMainWindow = new EmployeesMainWindow(this);
    stackedWidget->addWidget(employeesMainWindow);

    sponsorsMainWindow = new SponsorsMainWindow(this);
    stackedWidget->addWidget(sponsorsMainWindow);

    servicesMainWindow = new ServiceMainWindow(this);
    stackedWidget->addWidget(servicesMainWindow);

    evenementMainWindow = new EvenementMainWindow(this);
    stackedWidget->addWidget(evenementMainWindow);

    fournisseurMainWindow = new FournisseurMainWindow(this);
    stackedWidget->addWidget(fournisseurMainWindow);

    magasinMainWindow = new MagasinMainWindow(this);
    stackedWidget->addWidget(magasinMainWindow);

    // Set initial window
    stackedWidget->setCurrentWidget(employeesMainWindow);
}

void MainApplication::navigateTo(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
        // Update window title based on current widget
        switch(index) {
            case 0:
                setWindowTitle("MallFlow Management - Employees");
                break;
            case 1:
                setWindowTitle("MallFlow Management - Sponsors");
                break;
            case 2:
                setWindowTitle("MallFlow Management - Services");
                break;
            case 3:
                setWindowTitle("MallFlow Management - Events");
                break;
            case 4:
                setWindowTitle("MallFlow Management - Suppliers");
                break;
            case 5:
                setWindowTitle("MallFlow Management - Stores");
                break;
        }
    }
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
