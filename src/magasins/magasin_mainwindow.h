#ifndef MAGASIN_MAINWINDOW_H
#define MAGASIN_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSqlDatabase>
#include <QNetworkAccessManager>
#include "magasin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MagasinMainWindow; }
QT_END_NAMESPACE

class MagasinMainWindow : public QMainWindow {
    Q_OBJECT

public:
    MagasinMainWindow(QWidget *parent = nullptr);
    ~MagasinMainWindow();

signals:
    void navigateTo(int index);

private slots:
    void on_ajouterMagasin_clicked();
    void on_supprimerMagasin_clicked();
    void on_modifierMagasin_clicked();
    void on_rechercheButton_clicked();
    void on_triNomButton_clicked();
    void on_triPrixButton_clicked();
    void on_afficherButton_clicked();
    void on_fermerButton_clicked();
    void on_statistiquesButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void envoyerTousLesRappels();
    void appliquerSolde();
    void annulerSolde();
    void on_btnEmployees_clicked();
    void on_btnSponsors_clicked();
    void on_btnServices_clicked();
    void on_btnEvents_clicked();
    void on_btnSuppliers_clicked();

private:
    Ui::MagasinMainWindow *ui;
    QTimer *timer;
    QList<Magasin> magasins;
    QNetworkAccessManager *networkManager;

    void afficherMagasinsDansTable();
    void mettreAJourComboBox();
    double calculerMoyennePrix();
    double calculerPrixMinimum();
    double calculerPrixMaximum();
    double calculerEcartTypePrix();
    void exportToPDF(const QString &fileName);
    void envoyerEmailRappel(QString email, int id, QString nomMagasin, QString reference, double prix);
};

#endif // MAGASIN_MAINWINDOW_H