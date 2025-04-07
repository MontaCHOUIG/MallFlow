#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "evenement.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Sp_Button_Ajouter_clicked();
    void on_Sp_Button_Modifier_clicked();
    void on_Sp_Button_Supprimer_clicked();
    void on_Sp_Combo_ID_currentIndexChanged(const QString &idStr);

    // Slots ajoutés
    void on_Sp_Button_Recherche_clicked();
    void on_Sp_Button_Tri_Titre_clicked();
    void on_Sp_Button_Tri_Lieu_clicked();
    void on_Sp_Button_Tri_ID_clicked();
    void on_Sp_Button_PDF_clicked();

private:
    Ui::MainWindow *ui;
    QList<Evenement> evenements;

    void afficherEvenements(const QList<Evenement> &liste = QList<Evenement>());
    void genererPDF(const Evenement &e);
};

#endif // MAINWINDOW_H
