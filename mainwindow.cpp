#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modifieremploye.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Display employees in QTableView
    ui->Em_TableView->setModel(emp.afficher());

    connect(ui->Em_Button_ExportPDF, &QPushButton::clicked, this, &MainWindow::exportToPDF);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//  Load selected employee details into QLineEdits
void MainWindow::on_Em_TableView_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
        int row = index.row();
        ui->Em_Line_ID->setText(ui->Em_TableView->model()->index(row, 0).data().toString());
        ui->Em_Line_Nom->setText(ui->Em_TableView->model()->index(row, 1).data().toString());
        ui->Em_Line_role->setText(ui->Em_TableView->model()->index(row, 2).data().toString());
        ui->Em_Line_email->setText(ui->Em_TableView->model()->index(row, 3).data().toString());
        ui->Em_Line_salaire->setText(ui->Em_TableView->model()->index(row, 4).data().toString());
        ui->Em_Line_poste->setText(ui->Em_TableView->model()->index(row, 5).data().toString());
        ui->Em_Line_mdp->setText(ui->Em_TableView->model()->index(row, 6).data().toString());
    }
}

//  Add Employee
void MainWindow::on_Em_Button_Ajouter_clicked()
{
    int id = ui->Em_Line_ID->text().toInt();
    QString nom = ui->Em_Line_Nom->text();
    QString role = ui->Em_Line_role->text();
    QString email = ui->Em_Line_email->text();
    double salaire = ui->Em_Line_salaire->text().toDouble();
    QString poste = ui->Em_Line_poste->text();
    QString mdp = ui->Em_Line_mdp->text();

    Employe e(id, nom, role, email, poste, salaire, mdp);

    if (e.ajouter()) {
        qDebug() << "Employee added successfully!";
        ui->Em_TableView->setModel(emp.afficher()); // Refresh table
    } else {
        // qDebug() << "Failed to add employee.";
    }
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
    double salaire = ui->Em_TableView->model()->index(row, 4).data().toDouble();
    QString poste = ui->Em_TableView->model()->index(row, 5).data().toString();
    QString mdp = ui->Em_TableView->model()->index(row, 6).data().toString();

    // Open modification form
    ModifierEmploye modifyDialog(this);
    modifyDialog.setEmployeData(id, nom, role, email, salaire, poste, mdp);

    // Refresh table if modification is successful
    if (modifyDialog.exec() == QDialog::Accepted) {
        ui->Em_TableView->setModel(emp.afficher());
    }
}


// Delete Employee
void MainWindow::on_Em_Button_Supprimer_clicked()
{
    int id = ui->Em_Line_ID->text().toInt();

    if (id == 0) {
        QMessageBox::warning(this, "Delete Error", "Please select an employee first!");
        return;
    }

    if (emp.supprimer(id)) {
        qDebug() << "Employee deleted successfully!";
        ui->Em_TableView->setModel(emp.afficher()); // Refresh table
    } else {
        qDebug() << "Failed to delete employee.";
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
