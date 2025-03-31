#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modifieremploye.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>


#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize proxy model for filtering and sorting
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(emp.afficher());  // Use the existing model
    proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setDynamicSortFilter(true);

    // Set filtered model to TableView
    ui->Em_TableView->setModel(proxyModel);

    // Connect buttons
    connect(ui->Em_Button_ExportPDF, &QPushButton::clicked, this, &MainWindow::exportToPDF);


    connect(ui->Em_Button_ASCE, &QPushButton::clicked, this, &MainWindow::sortBySalaryAscending);
    connect(ui->Em_Button_DESC, &QPushButton::clicked, this, &MainWindow::sortBySalaryDescending);

    // Connect search bar
    connect(ui->Em_Line_Recherche, &QLineEdit::textChanged, this, &MainWindow::on_Em_Line_Search_textChanged);

    showSalaryPieChart();

}



MainWindow::~MainWindow()
{
    delete ui;
}





//  Add Employee
void MainWindow::on_Em_Button_Ajouter_clicked()
{
    bool ok;

    // Validation de l'ID
    int id = ui->Em_Line_ID->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    // Vérification que le nom n'est pas vide
    QString nom = ui->Em_Line_Nom->text();
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide !");
        return;
    }

    // Vérification que le rôle n'est pas vide
    QString role = ui->Em_Line_role->text();
    if (role.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le rôle ne peut pas être vide !");
        return;
    }


    QString email = ui->Em_Line_email->text().trimmed();

    // Une regex pour l'email
    QRegularExpression emailRegex("^[\\w\\.\\-]+@[\\w\\-]+(\\.[\\w\\-]{2,})+$");
    QRegularExpressionMatch match = emailRegex.match(email);

    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Format de l'email invalide !");
        return;
    }


    // Validation du salaire
    double salaire = ui->Em_Line_salaire->text().toDouble(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::warning(this, "Erreur", "Salaire invalide !");
        return;
    }

    // Vérification que le poste n'est pas vide
    QString poste = ui->Em_Line_poste->text();
    if (poste.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le poste ne peut pas être vide !");
        return;
    }

    // Vérification que le mot de passe n'est pas vide
    QString mdp = ui->Em_Line_mdp->text();
    if (mdp.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide !");
        return;
    }

    // Création de l'objet Employe
    Employe e(id, nom, role, email, poste, salaire, mdp);

    // Tentative d'ajout de l'employé et affichage du résultat
    if (e.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
         proxyModel->setSourceModel(emp.afficher()); // Rafraîchissement du tableau
        showSalaryPieChart();  // Update pie chart
        clearEmployeeForm();
    } else {
        QMessageBox::warning(this, "Erreur", "L'ajout de l'employé a échoué !");
    }
}

void MainWindow::clearEmployeeForm()
{
    ui->Em_Line_ID->clear();
    ui->Em_Line_Nom->clear();
    ui->Em_Line_role->clear();
    ui->Em_Line_email->clear();
    ui->Em_Line_salaire->clear();
    ui->Em_Line_poste->clear();
    ui->Em_Line_mdp->clear();
}


// Open the Modify Employee Dialog
void MainWindow::on_Em_Button_Modifier_clicked()
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
        showSalaryPieChart();  // Update pie chart

    }
}


// Delete Employee
void MainWindow::on_Em_Button_Supprimer_clicked()
{
    QModelIndex index = ui->Em_TableView->currentIndex();

    // Check if a valid row is selected
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur de suppression", "Veuillez sélectionner un employé dans le tableau !");
        return;
    }

    int row = index.row();
    int id = ui->Em_TableView->model()->index(row, 0).data().toInt(); // Get ID from the selected row

    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cet employé ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (emp.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
             proxyModel->setSourceModel(emp.afficher()); // Refresh table
            showSalaryPieChart();  // Update pie chart

        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression de l'employé.");
        }
    }
}



void MainWindow::exportToPDF() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300); // High resolution
    QPainter painter(&writer);

    if (!painter.isActive()) {
        QMessageBox::warning(this, "Error", "Could not open PDF for writing.");
        return;
    }

    int marginLeft = 100; // Left margin
    int startY = 200;     // Start position (higher for better spacing)
    int rowHeight = 200;   // More height for rows
    int columnSpacing = 350; // More space between columns

    // Set Title
    QFont titleFont("Arial", 12, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(marginLeft, startY, "Liste des Employés");

    startY += 80; // More space after title

    // Column headers
    QFont headerFont("Arial", 12, QFont::Bold);
    painter.setFont(headerFont);
    painter.drawText(marginLeft, startY, "ID");
    painter.drawText(marginLeft + 0.5 * columnSpacing, startY, "NOM");
    painter.drawText(marginLeft + 1.5 * columnSpacing, startY, "POSTE");
    painter.drawText(marginLeft + 2.2 * columnSpacing, startY, "EMAIL");
    painter.drawText(marginLeft + 4 * columnSpacing, startY, "RÔLE");
    painter.drawText(marginLeft + 5 * columnSpacing, startY, "SALAIRE");
    painter.drawText(marginLeft + 6 * columnSpacing, startY, "MDP");

    startY += 30; // More space before table data

    // Set font for data
    QFont dataFont("Courier New", 11);
    painter.setFont(dataFont);

    // Get data from QTableView
    QAbstractItemModel *model = ui->Em_TableView->model();
    for (int row = 0; row < model->rowCount(); row++) {
        // Draw employee data with more spacing
        painter.drawText(marginLeft, startY, model->index(row, 0).data().toString());
        painter.drawText(marginLeft + 0.5 * columnSpacing, startY, model->index(row, 1).data().toString());
        painter.drawText(marginLeft + 1.5 * columnSpacing, startY, model->index(row, 2).data().toString());
        painter.drawText(marginLeft + 2.2 * columnSpacing, startY, model->index(row, 3).data().toString());
        painter.drawText(marginLeft + 4 * columnSpacing, startY, model->index(row, 4).data().toString());
        painter.drawText(marginLeft + 5 * columnSpacing, startY, model->index(row, 5).data().toString());
        painter.drawText(marginLeft + 6 * columnSpacing, startY, model->index(row, 6).data().toString());

        startY += rowHeight; // Move to next row with more space
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF généré avec succès !");
}

void MainWindow::sortBySalaryAscending()
{
    proxyModel->sort(5, Qt::AscendingOrder); // Column index 4 is for salary
}

void MainWindow::sortBySalaryDescending()
{
    proxyModel->sort(5, Qt::DescendingOrder); // Column index 4 is for salary
}

void MainWindow::on_Em_Line_Search_textChanged(const QString &text)
{
    proxyModel->setFilterKeyColumn(1);
    proxyModel->setFilterFixedString(text);
}


void MainWindow::showSalaryPieChart()
{
    // Count employees based on salary ranges
    int low = 0, medium = 0, high = 0;
    QAbstractItemModel *model = ui->Em_TableView->model();
    if (!model) return;

    for (int row = 0; row < model->rowCount(); ++row) {
        double salary = model->index(row, 5).data().toDouble(); // Assuming column 5 is salary
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





