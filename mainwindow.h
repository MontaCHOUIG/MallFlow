#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sponsor.h" // Changed from "client.h" to "sponsor.h"
#include <QList>
#include <QString>
#include <QSqlQuery>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
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

    void on_Sp_Button_Ajouter_clicked();

    void on_Sp_Button_Modifier_clicked();

    void on_Sp_Button_Supprimer_clicked();

    void on_Sp_Combo_IDs_currentIndexChanged(int index);

    void on_Sp_Button_ExportPDF_clicked();

    void on_Sp_Button_Tri_Nom_clicked();

    void on_Sp_Button_Tri_Email_clicked();

    void on_Sp_Button_Tri_Numtel_clicked();

    void on_Sp_Line_Recherche_textChanged(const QString &arg1);

    void on_pushButton_Stat_clicked();

    void tech_choix_pie();

private:
    Ui::MainWindow *ui;
    Sponsor S; // Changed from Client C to Sponsor S
    void clearFields();
};
#endif // MAINWINDOW_H
