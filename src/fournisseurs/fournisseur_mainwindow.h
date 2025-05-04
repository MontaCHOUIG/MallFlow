#ifndef FOURNISSEUR_MAINWINDOW_H
#define FOURNISSEUR_MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include "fournisseur.h"

class fournisseur;

QT_BEGIN_NAMESPACE
namespace Ui { class FournisseurMainWindow; }
QT_END_NAMESPACE

class FournisseurMainWindow : public QMainWindow {
    Q_OBJECT

public:
    FournisseurMainWindow(QWidget *parent = nullptr);
    ~FournisseurMainWindow();

private slots:
    void on_pb_ajouter_clicked();
    void on_pb_supprimer_clicked();
    void on_pb_modifier_clicked();
    void on_pb_id_clicked();
    void on_pb_tri_parnom_clicked();
    void on_pb_tri_type_clicked();
    void on_lineEditRecherche_textChanged(const QString &text);
    void on_pb_refresh_stats_clicked();
    void on_pb_importer_clicked();
    void on_Sp_Button_ExportPDF_clicked();

private:
    Ui::FournisseurMainWindow *ui;
    fournisseur f;

    QString generateCaptchaText(int length);
    bool verifyCaptcha();
    void updateStatsChart();
};

#endif // FOURNISSEUR_MAINWINDOW_H
