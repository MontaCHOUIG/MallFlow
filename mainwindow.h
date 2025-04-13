#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QTableView> // Added for styleTableView

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    // Slots CRUD
    void onBtnAjouterClicked();
    void onBtnModifierClicked();
    void onBtnSupprimerClicked();
    void onBtnAfficherClicked();
    void onBtnTriNomClicked();
    void onBtnTriTypeClicked();
    void onRechercheTextChanged(const QString &text);
    void on_Sp_Button_auto_clicked();

    // Slots supplémentaires
    void on_Sp_Button_Modifier_2_clicked();  // Historique
    void on_Sp_Button_Fermer_clicked();      // Fermer
    void on_Sp_Button_Fermer_2_clicked();    // Fermer
    void on_Sp_Button_Fermer_4_clicked();    // Fermer
    void on_Sp_Button_ExportPDF_clicked();   // Export PDF
    void refreshStats();                     // Actualiser stats
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QChartView *chartView;
    int getSelectedServiceID();
    void loggerHistorique(const QString& action, int id_service, const QString& details);
    void updateChart();
    void styleTableView(QTableView *tableView);
};

#endif // MAINWINDOW_H
