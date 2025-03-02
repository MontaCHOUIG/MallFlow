#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Display employees in QTableView
    ui->Em_TableView->setModel(emp.afficher());


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

// Modify Employee
void MainWindow::on_Em_Button_Modifier_clicked()
{
    int id = ui->Em_Line_ID->text().toInt();
    QString nom = ui->Em_Line_Nom->text();
    QString role = ui->Em_Line_role->text();
    QString email = ui->Em_Line_email->text();
    double salaire = ui->Em_Line_salaire->text().toDouble();
    QString poste = ui->Em_Line_poste->text();
    QString mdp = ui->Em_Line_mdp->text();

    Employe e(id, nom, role, email, poste, salaire, mdp);

    if (e.modifier()) {
        qDebug() << "Employee updated successfully!";
        ui->Em_TableView->setModel(emp.afficher()); // Refresh table
    } else {
        qDebug() << "Failed to update employee.";
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
