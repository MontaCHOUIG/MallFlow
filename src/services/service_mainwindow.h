#ifndef SERVICE_MAINWINDOW_H
#define SERVICE_MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QTableView> // Added for styleTableView

QT_BEGIN_NAMESPACE
namespace Ui { class ServiceMainWindow; }
QT_END_NAMESPACE

class ServiceMainWindow : public QMainWindow {
    Q_OBJECT
public:
    ServiceMainWindow(QWidget *parent = nullptr);
    ~ServiceMainWindow();
protected:
    void showEvent(QShowEvent *event) override;
private slots:
    void onBtnAjouterClicked();
    void onBtnAfficherClicked();
    void onBtnSupprimerClicked();
    void onBtnModifierClicked();
    void on_pushButton_2_clicked();
    void onBtnTriNomClicked();
    void onBtnTriTypeClicked();
    void onRechercheTextChanged(const QString &text);
    void on_Sp_Button_auto_clicked();
    void on_Sp_Button_ExportPDF_clicked();
    void on_Sp_Button_Modifier_2_clicked();
    void on_Sp_Button_Fermer_clicked();
    void on_Sp_Button_Fermer_2_clicked();
    void on_Sp_Button_Fermer_4_clicked();
    void refreshStats();
private:
    Ui::ServiceMainWindow *ui;
    QChartView *chartView;
    void styleTableView(QTableView *tableView);
    void updateChart();
    void loggerHistorique(const QString& action, int id_service, const QString& details);
    int getSelectedServiceID();
};

#endif
