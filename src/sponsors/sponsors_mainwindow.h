#ifndef SPONSORS_MAINWINDOW_H
#define SPONSORS_MAINWINDOW_H

#include <QMainWindow>
#include "sponsor.h"
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
namespace Ui { class SponsorsMainWindow; }
QT_END_NAMESPACE

class SponsorsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SponsorsMainWindow(QWidget *parent = nullptr);
    ~SponsorsMainWindow();

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
    void on_calendarWidget_sp_selectionChanged();
    void highlightDates();
    void checkDateFinAndSendMessage();

private:
    Ui::SponsorsMainWindow *ui;
    Sponsor S;
    void clearFields();
};

#endif // SPONSORS_MAINWINDOW_H