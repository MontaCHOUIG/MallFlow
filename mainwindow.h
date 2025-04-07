#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fournisseur.h"

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

    void on_pb_ajouter_clicked();


    void on_pb_supprimer_clicked();



    void on_pb_modifier_clicked();

    void on_Sp_Button_ExportPDF_clicked();

    void on_pb_id_clicked();



    void on_pb_tri_parnom_clicked();

    void on_pb_tri_contrat_clicked();

private:
    Ui::MainWindow *ui;
    fournisseur f ;
};
#endif // MAINWINDOW_H
