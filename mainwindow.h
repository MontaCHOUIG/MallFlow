#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>

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
    void onBtnAjouterClicked();
    void onBtnModifierClicked();
    void onBtnSupprimerClicked();
    void onBtnAfficherClicked();
    void onBtnTriNomClicked();
    void onBtnTriTypeClicked();
    void onRechercheTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    int getSelectedServiceID(); // Méthode pour récupérer l'ID du service sélectionné
};

#endif // MAINWINDOW_H
