#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtCharts/QChartView>
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
    void readSerialData();
    void on_pb_ajouter_clicked();
    void on_pb_supprimer_clicked();
    void on_pb_modifier_clicked();
    void on_pb_id_clicked();
    void on_pb_tri_parnom_clicked();
    void on_lineEditRecherche_textChanged(const QString &text);
    void on_pb_importer_clicked();
    void on_Sp_Button_ExportPDF_clicked();
    void on_pb_refresh_stats_clicked();

private:
    void setupSerialPort();
    void checkRFID(const QString &uid);
    void updateStatsChart();
    QString generateCaptchaText(int length);
    bool verifyCaptcha();
    bool isValidRFID(const QString &uid); // Nouvelle fonction pour valider les UID
    Ui::MainWindow *ui;
    fournisseur f;
    QSerialPort *serialPort;
    QString lastUid;
};

#endif // MAINWINDOW_H
