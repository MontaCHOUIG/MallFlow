#include "main_application.h"
#include <QFile>
#include <QDebug>
#include "src/employees/login.h"

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

    // connect logout signals
    connect(employeesMainWindow, &EmployeesMainWindow::logoutRequested, this, &MainApplication::handleLogout);
    connect(evenementMainWindow, &EvenementMainWindow::logoutRequested, this, &MainApplication::handleLogout);
    connect(fournisseurMainWindow, &FournisseurMainWindow::logoutRequested, this, &MainApplication::handleLogout);
    connect(magasinMainWindow, &MagasinMainWindow::logoutRequested, this, &MainApplication::handleLogout);
    connect(servicesMainWindow, &ServiceMainWindow::logoutRequested, this, &MainApplication::handleLogout);
    connect(sponsorsMainWindow, &SponsorsMainWindow::logoutRequested, this, &MainApplication::handleLogout);

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

    // Determine initial window based on user permissions
    Employe emp;
    int initialWindowIndex = 0; // Default to employees window

    // Check each window's access in order of priority
    if (emp.hasWindowAccess("employees")) {
        initialWindowIndex = 0;
    } else if (emp.hasWindowAccess("sponsors")) {
        initialWindowIndex = 1;
    } else if (emp.hasWindowAccess("services")) {
        initialWindowIndex = 2;
    } else if (emp.hasWindowAccess("evenements")) {
        initialWindowIndex = 3;
    } else if (emp.hasWindowAccess("fournisseurs")) {
        initialWindowIndex = 4;
    } else if (emp.hasWindowAccess("magasins")) {
        initialWindowIndex = 5;
    }

    // Set initial window
    stackedWidget->setCurrentIndex(initialWindowIndex);
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
                setWindowTitle("MallFlow Management - Fournisseurs");
                break;
            case 5:
                setWindowTitle("MallFlow Management - Magasins");
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

void MainApplication::handleLogout() {
    Employe emp;
    emp.logoutUser(); // Logout the user

    // Close the MainApplication (and all child windows)
    close();

    // Show the login screen
    Login login;
    if (login.exec() == QDialog::Accepted) {
        // Recreate and show a new MainApplication instance
        MainApplication* newMainApp = new MainApplication();
        newMainApp->show();
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
