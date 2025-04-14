#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QQuickWidget>
#include <QGeoCoordinate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>



namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Boutons
    void on_Sp_Button_Ajouter_clicked();
    void on_Sp_Button_Modifier_clicked();
    void on_Sp_Button_Supprimer_clicked();
    void on_Sp_Button_Recherche_clicked();
    void on_Sp_Button_Tri_ID_clicked();
    void on_Sp_Button_Tri_Titre_clicked();
    void on_Sp_Button_Tri_Lieu_clicked();
    void on_Sp_Button_PDF_clicked();
    void onQRCodeDownloaded(QNetworkReply* reply);


    // Combo box
    void on_Sp_combo_ID_currentIndexChanged(int index);
    void on_tab4_combo_Event_currentIndexChanged(int index);



private:
    Ui::MainWindow *ui;

    // Fonctions utilitaires
    bool recordExists(const QString &table, const QString &col, int id);
    void afficherEvenements(const QString &orderBy = "");
    void genererPDF(const QString &fileName);
    void clearEventFields();
    void updateStats();
    QChartView *chartViewParticipants;
    QChartView *chartViewLieux;
    QQuickWidget *mapWidget;
    QNetworkAccessManager *networkManager;
    void geocodeAddress(const QString &address);
    void generateQRCodeViaAPI(const QString &data);
    void updateQRCodeComboBox();


};

#endif // MAINWINDOW_H
