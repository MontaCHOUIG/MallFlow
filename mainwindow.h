#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "employe.h"
#include <QSortFilterProxyModel>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <arduino.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateAuthUserLabel();
private slots:
    void on_Em_Button_Ajouter_clicked();  // Add employee
    void on_Em_Button_Modifier_clicked(); // Modify employee
    void on_Em_Button_Supprimer_clicked(); // Delete employee
    void sortBySalaryAscending();
    void sortBySalaryDescending();
    void clearEmployeeForm();
    void on_button_logout_clicked();
    void handleArduinoId(const QString &id) ;


public slots:
    void exportToPDF(const QString &fileName);
    void showSalaryPieChart();

private:
    Ui::MainWindow *ui;
    Employe emp;
    QSortFilterProxyModel *proxyModel;
    QString authUserEmail;  
    Arduino *arduino;
};

#endif // MAINWINDOW_H
