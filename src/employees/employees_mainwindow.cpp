#include "employees_mainwindow.h"
#include "ui_employees.h"
#include "modifieremploye.h"
#include "securityquestion.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QPrinter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QDesktopServices>
#include "login.h"
#include "employe.h"

EmployeesMainWindow::EmployeesMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeesMainWindow)
{
    ui->setupUi(this);

    // Initialize proxy model for filtering and sorting
    proxyModel = new QSortFilterProxyModel(this);
    QSqlQueryModel *model = emp.afficher();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Poste");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Rôle");
    model->setHeaderData(5, Qt::Horizontal, "Salaire");
    model->setHeaderData(6, Qt::Horizontal, "Mot de passe ");

    proxyModel->setSourceModel(model);
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setDynamicSortFilter(true);
    updateAuthUserLabel();

    // Set filtered model to TableView
    ui->Em_TableView->setModel(proxyModel);

    // Connect buttons
    connect(ui->Em_Button_ExportPDF, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            exportToPDF(fileName);
        }
    });

    connect(ui->Em_Button_ASCE, &QPushButton::clicked, this, &EmployeesMainWindow::sortBySalaryAscending);
    connect(ui->Em_Button_DESC, &QPushButton::clicked, this, &EmployeesMainWindow::sortBySalaryDescending);

    // Connect search bar
    connect(ui->Em_Line_Recherche, &QLineEdit::textChanged, this, &EmployeesMainWindow::on_Em_Line_Search_textChanged);

    showSalaryPieChart();
}

EmployeesMainWindow::~EmployeesMainWindow()
{
    delete ui;
}

void EmployeesMainWindow::on_Em_Button_Ajouter_clicked()
{
    bool ok;

    // Validation de l'ID
    int id = ui->Em_Line_ID->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    QString nom = ui->Em_Line_Nom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide !");
        return;
    }

    QString role = ui->Em_Line_role->text().trimmed();
    if (role.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le rôle ne peut pas être vide !");
        return;
    }

    QString email = ui->Em_Line_email->text().trimmed();
    QRegularExpression emailRegex("^[\\w\\.\\-]+@[\\w\\-]+(\\.[\\w\\-]{2,})+$");
    QRegularExpressionMatch match = emailRegex.match(email);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Format de l'email invalide !");
        return;
    }

    double salaire = ui->Em_Line_salaire->text().toDouble(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::warning(this, "Erreur", "Salaire invalide !");
        return;
    }

    QString poste = ui->Em_Line_poste->text().trimmed();
    if (poste.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le poste ne peut pas être vide !");
        return;
    }

    QString mdp = ui->Em_Line_mdp->text().trimmed();
    if (mdp.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide !");
        return;
    }

    // Ask if they want to enable security questions
    int choice = QMessageBox::question(
        this,
        "Sécurité supplémentaire",
        "Souhaitez-vous activer l'authentification par questions de sécurité ?",
        QMessageBox::Yes | QMessageBox::No
    );

    int secureAuth = 0;
    QString question1, answer1, question2, answer2;

    if (choice == QMessageBox::Yes) {
        SecurityQuestion dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            secureAuth = 1;
            question1 = dialog.getQuestion1();
            answer1 = dialog.getAnswer1();
            question2 = dialog.getQuestion2();
            answer2 = dialog.getAnswer2();
        } else {
            QMessageBox::information(this, "Annulé", "Ajout annulé car les questions n'ont pas été complétées.");
            return;
        }
    }

    // Create the employee
    Employe e(id, nom, role, email, poste, salaire, mdp, secureAuth);

    if (e.ajouter()) {
        // Save security questions only if used
        if (secureAuth == 1) {
            e.setSecurityQuestions(question1, answer1, question2, answer2);
        }

        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        proxyModel->setSourceModel(emp.afficher());
        showSalaryPieChart();
        clearEmployeeForm();
    } else {
        QMessageBox::warning(this, "Erreur", "L'ajout de l'employé a échoué !");
    }
}

void EmployeesMainWindow::clearEmployeeForm()
{
    ui->Em_Line_ID->clear();
    ui->Em_Line_Nom->clear();
    ui->Em_Line_role->clear();
    ui->Em_Line_email->clear();
    ui->Em_Line_salaire->clear();
    ui->Em_Line_poste->clear();
    ui->Em_Line_mdp->clear();
}

void EmployeesMainWindow::on_Em_Button_Modifier_clicked()
{
    QModelIndex index = ui->Em_TableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur de sélection", "Veuillez sélectionner un employé !");
        return;
    }

    int row = index.row();
    int id = ui->Em_TableView->model()->index(row, 0).data().toInt();
    QString nom = ui->Em_TableView->model()->index(row, 1).data().toString();
    QString role = ui->Em_TableView->model()->index(row, 2).data().toString();
    QString email = ui->Em_TableView->model()->index(row, 3).data().toString();
    QString poste = ui->Em_TableView->model()->index(row, 4).data().toString();
    double salaire = ui->Em_TableView->model()->index(row, 5).data().toDouble();
    QString mdp = ui->Em_TableView->model()->index(row, 6).data().toString();

    // Open modification form
    ModifierEmploye modifyDialog(this);
    modifyDialog.setEmployeData(id, nom, role, email, salaire, poste, mdp);

    // Refresh table if modification is successful
    if (modifyDialog.exec() == QDialog::Accepted) {
        proxyModel->setSourceModel(emp.afficher());
        showSalaryPieChart();
    }
}

void EmployeesMainWindow::on_Em_Button_Supprimer_clicked()
{
    QModelIndex index = ui->Em_TableView->currentIndex();

    // Check if a valid row is selected
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur de suppression", "Veuillez sélectionner un employé dans le tableau !");
        return;
    }

    int row = index.row();
    int id = ui->Em_TableView->model()->index(row, 0).data().toInt();

    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (emp.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
            proxyModel->setSourceModel(emp.afficher());
            showSalaryPieChart();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression de l'employé.");
        }
    }
}

void EmployeesMainWindow::exportToPDF(const QString &fileName)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 20, 15, 15));

    // Entête avec logo à gauche + titre/date centrés
    QString html = "<div style='display: flex; margin-bottom: 25px; padding-bottom: 15px; border-bottom: 2px solid #2E86C1;'>"
                   "<img src=':/imgs/logo.png' width='120' style='margin-right: 30px;'/>"
                   "<div style='flex-grow: 1; text-align: center;'>"
                   "<h1 style='color: #2E86C1; margin: 0; font-family: Arial;'>Liste des Employés</h1>"
                   "<p style='color: #6C757D; font-size: 12px; margin-top: 3px;'>"
                   "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>"
                   "</div></div>";

    // Style du tableau
    html += "<style>"
            "table { margin: 0 auto; width: 98%; border-collapse: collapse; border: 1px solid #ddd; }"
            "th, td { padding: 10px; border: 1px solid #ccc; text-align: center; font-family: Arial; font-size: 11pt; }"
            "td:nth-child(2) { text-align: left; }"
            "th { background-color: #f8f9fa; color: #2E86C1; font-size: 12pt; }"
            "</style>";

    // Entête du tableau
    html += "<table>"
            "<tr>"
            "<th>ID</th><th>Nom</th><th>Poste</th><th>Email</th>"
            "<th>Rôle</th><th>Salaire</th>"
            "</tr>";

    // Récupération des données depuis le QTableView
    QAbstractItemModel *model = ui->Em_TableView->model();
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < 6; ++col) {
            QString data = model->index(row, col).data().toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";

    // Création du document PDF
    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void EmployeesMainWindow::sortBySalaryAscending()
{
    proxyModel->sort(5, Qt::AscendingOrder);
}

void EmployeesMainWindow::sortBySalaryDescending()
{
    proxyModel->sort(5, Qt::DescendingOrder);
}

void EmployeesMainWindow::on_Em_Line_Search_textChanged(const QString &text)
{
    proxyModel->setFilterKeyColumn(1);
    proxyModel->setFilterFixedString(text);
}

void EmployeesMainWindow::showSalaryPieChart()
{
    // Count employees based on salary ranges
    int low = 0, medium = 0, high = 0;
    QAbstractItemModel *model = ui->Em_TableView->model();
    if (!model) return;

    for (int row = 0; row < model->rowCount(); ++row) {
        double salary = model->index(row, 5).data().toDouble();
        if (salary < 2000)
            low++;
        else if (salary <= 5000)
            medium++;
        else
            high++;
    }

    // Create a pie chart series
    QPieSeries *series = new QPieSeries();
    series->append(QString("Low (-2000)"), low);
    series->append(QString("Medium (2000-5000)"), medium);
    series->append(QString("High (>5000)"), high);

    // Customize slices
    for (QPieSlice *slice : series->slices()) {
        QString formattedLabel = QString("%1: %2 employees")
            .arg(slice->label())
            .arg(QString::number(slice->value()));
        slice->setLabel(formattedLabel);
        slice->setLabelVisible(true);
    }

    // Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Salary Distribution");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create the chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Remove previous layout if any and set a new one
    QLayout *oldLayout = ui->chartWidget->layout();
    if (oldLayout) {
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout(ui->chartWidget);
    layout->addWidget(chartView);
    ui->chartWidget->setLayout(layout);
}

void EmployeesMainWindow::updateAuthUserLabel()
{
    authUserEmail = emp.getAuthenticatedUser();
    ui->label_authUser->setText("Logged in as: " + authUserEmail);
}

void EmployeesMainWindow::on_button_logout_clicked()
{
    emp.logoutUser();
    close();

    // Show login screen again
    Login login;
    if (login.exec() == QDialog::Accepted) {
        updateAuthUserLabel();
        show();
    }
}
