#include "modifieremploye.h"
#include <QMessageBox>
#include <QVBoxLayout>

ModifierEmploye::ModifierEmploye(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Modifier Employé");
    resize(350, 450);

    // Create UI elements dynamically
    lineNom = new QLineEdit(this);
    lineRole = new QLineEdit(this);
    lineEmail = new QLineEdit(this);
    lineSalaire = new QLineEdit(this);
    linePoste = new QLineEdit(this);
    lineMdp = new QLineEdit(this);
    saveButton = new QPushButton("Enregistrer", this);

    // Apply placeholder text
    lineNom->setPlaceholderText("Nom de l'employé");
    lineRole->setPlaceholderText("Rôle");
    lineEmail->setPlaceholderText("Email");
    lineSalaire->setPlaceholderText("Salaire");
    linePoste->setPlaceholderText("Poste");
    lineMdp->setPlaceholderText("Mot de passe");

    // Apply Styles (QSS)
    setStyleSheet(
        "QDialog { background-color: #f4f4f4; border-radius: 10px; }"
        "QLineEdit { padding: 8px; border: 2px solid #3498db; border-radius: 5px; font-size: 14px; }"
        "QLineEdit:focus { border: 2px solid #2980b9; }"
        "QPushButton { background-color: #3498db; color: white; padding: 10px; font-size: 14px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        );

    // Arrange UI elements with layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(lineNom);
    layout->addWidget(lineRole);
    layout->addWidget(lineEmail);
    layout->addWidget(lineSalaire);
    layout->addWidget(linePoste);
    layout->addWidget(lineMdp);
    layout->addWidget(saveButton);

    setLayout(layout);

    // Connect save button to function
    connect(saveButton, &QPushButton::clicked, this, &ModifierEmploye::on_saveButton_clicked);
}

ModifierEmploye::~ModifierEmploye() {}

// Set employee data when opening the form
void ModifierEmploye::setEmployeData(int id, QString nom, QString role, QString email, double salaire, QString poste, QString mdp)
{
    empId = id;
    lineNom->setText(nom);
    lineRole->setText(role);
    lineEmail->setText(email);
    lineSalaire->setText(QString::number(salaire));
    linePoste->setText(poste);
    lineMdp->setText(mdp);
}

// Save modifications
void ModifierEmploye::on_saveButton_clicked()
{
    QString nom = lineNom->text();
    QString role = lineRole->text();
    QString email = lineEmail->text();
    double salaire = lineSalaire->text().toDouble();
    QString poste = linePoste->text();
    QString mdp = lineMdp->text();

    Employe e(empId, nom, role, email, poste, salaire, mdp);

    if (e.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        accept(); // Close the dialog
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'employé.");
    }
}
