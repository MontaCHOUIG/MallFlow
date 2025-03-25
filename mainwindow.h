#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "employe.h"
#include <QSortFilterProxyModel>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Em_Button_Ajouter_clicked();  // Add employee
    void on_Em_Button_Modifier_clicked(); // Modify employee
    void on_Em_Button_Supprimer_clicked(); // Delete employee
    void sortBySalaryAscending();
    void sortBySalaryDescending();
    void on_Em_Line_Search_textChanged(const QString &text);
    void clearEmployeeForm();



public slots:
    void exportToPDF();
    void showSalaryPieChart();

private:
    Ui::MainWindow *ui;
    Employe emp;
    QSortFilterProxyModel *proxyModel;
};

#endif // MAINWINDOW_H
