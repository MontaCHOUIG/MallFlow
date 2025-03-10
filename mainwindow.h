#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "employe.h"

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
    void on_Em_TableView_clicked(const QModelIndex &index); // Select row
    void on_Em_Button_Ajouter_clicked();  // Add employee
    void on_Em_Button_Modifier_clicked(); // Modify employee
    void on_Em_Button_Supprimer_clicked(); // Delete employee


public slots:
    void exportToPDF();

private:
    Ui::MainWindow *ui;
    Employe emp;
};

#endif // MAINWINDOW_H
