#ifndef MODIFIEREMPLOYE_H
#define MODIFIEREMPLOYE_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "employe.h"

class ModifierEmploye : public QDialog
{
    Q_OBJECT

public:
    explicit ModifierEmploye(QWidget *parent = nullptr);
    ~ModifierEmploye();

    void setEmployeData(int id, QString nom, QString role, QString email, double salaire, QString poste, QString mdp);

private slots:
    void on_saveButton_clicked(); // Save modifications

private:
    int empId; // Store employee ID

    // Dynamically created UI elements
    QLineEdit *lineNom;
    QLineEdit *lineRole;
    QLineEdit *lineEmail;
    QLineEdit *lineSalaire;
    QLineEdit *linePoste;
    QLineEdit *lineMdp;
    QPushButton *saveButton;
};

#endif // MODIFIEREMPLOYE_H
