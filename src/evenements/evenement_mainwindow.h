#ifndef EVENEMENT_MAINWINDOW_H
#define EVENEMENT_MAINWINDOW_H

#include <QMainWindow>
#include <QQuickWidget>
#include <QChartView>
#include <QNetworkAccessManager>
#include "evenement.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EvenementMainWindow; }
QT_END_NAMESPACE

class EvenementMainWindow : public QMainWindow {
    Q_OBJECT
public:
    EvenementMainWindow(QWidget *parent = nullptr);
    ~EvenementMainWindow();

private slots:
    void on_Sp_Button_Ajouter_clicked();
    void on_Sp_Button_Modifier_clicked();
    void on_Sp_Button_Supprimer_clicked();
    void on_Sp_combo_ID_currentIndexChanged(int index);
    void on_Sp_Button_Recherche_clicked();
    void on_Sp_Button_Tri_ID_clicked();
    void on_Sp_Button_Tri_Titre_clicked();
    void on_Sp_Button_Tri_Lieu_clicked();
    void on_Sp_Button_PDF_clicked();
    void onNetworkReplyFinished(QNetworkReply *reply);
    void on_tab4_combo_Event_currentIndexChanged(int index);

private:
    Ui::EvenementMainWindow *ui;
    QQuickWidget *mapWidget;
    QChartView *chartViewParticipants;
    QChartView *chartViewLieux;
    QNetworkAccessManager *networkManager;
    QUrl currentGeocodeUrl;
    QUrl currentQRCodeUrl;

    bool recordExists(const QString &table, const QString &col, int id);
    void afficherEvenements(const QString &orderBy = QString());
    void genererPDF(const QString &fileName);
    void updateStats();
    void geocodeAddress(const QString &address);
    void generateQRCodeViaAPI(const QString &data);
    void updateQRCodeComboBox();
};

#endif // EVENEMENT_MAINWINDOW_H
