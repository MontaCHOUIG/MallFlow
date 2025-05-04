#include "modifieremploye.h"
#include "qregularexpression.h"
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

void ModifierEmploye::on_saveButton_clicked()
{
    bool ok;

    // Récupération et nettoyage des données
    QString nom = lineNom->text().trimmed();
    if(nom.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Le nom ne peut pas être vide !");
        return;
    }

    QString role = lineRole->text().trimmed();
    if(role.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Le rôle ne peut pas être vide !");
        return;
    }

    QString email = lineEmail->text().trimmed();
    // Validation du format de l'email avec une regex plus flexible
    QRegularExpression emailRegex("^[\\w\\.\\-]+@[\\w\\-]+(\\.[\\w\\-]{2,})+$");
    QRegularExpressionMatch match = emailRegex.match(email);
    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Format de l'email invalide !");
        return;
    }

    double salaire = lineSalaire->text().toDouble(&ok);
    if (!ok || salaire < 0) {
        QMessageBox::warning(this, "Erreur", "Salaire invalide !");
        return;
    }

    QString poste = linePoste->text().trimmed();
    if(poste.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Le poste ne peut pas être vide !");
        return;
    }

    QString mdp = lineMdp->text().trimmed();
    if(mdp.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Le mot de passe ne peut pas être vide !");
        return;
    }

    // Création de l'objet Employe avec les données validées
    Employe e(empId, nom, role, email, poste, salaire, mdp, 1);

    // Tentative de modification de l'employé dans la base
    if (e.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        accept(); // Ferme la fenêtre de dialogue
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'employé.");
    }
}

