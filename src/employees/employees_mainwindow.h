#ifndef EMPLOYEES_MAINWINDOW_H
#define EMPLOYEES_MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include "employe.h"

class ModifierEmploye;
class SecurityQuestion;
class Login;

namespace Ui {
class EmployeesMainWindow;
}

class EmployeesMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EmployeesMainWindow(QWidget *parent = nullptr);
    ~EmployeesMainWindow();

private slots:
    void on_Em_Button_Ajouter_clicked();
    void on_Em_Button_Modifier_clicked();
    void on_Em_Button_Supprimer_clicked();
    void sortBySalaryAscending();
    void sortBySalaryDescending();
    void on_Em_Line_Search_textChanged(const QString &text);
    void updateAuthUserLabel();
    void on_button_logout_clicked();

private:
    Ui::EmployeesMainWindow *ui;
    QSortFilterProxyModel *proxyModel;
    Employe emp;
    QString authUserEmail;

    void clearEmployeeForm();
    void exportToPDF(const QString &fileName);
    void showSalaryPieChart();
};

#endif