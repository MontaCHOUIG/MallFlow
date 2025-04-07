#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QSqlQueryModel>

class fournisseur
{
public:
    fournisseur();
    fournisseur(int, QString, QString, QString, QString, QString);

    int getIdFournisseur();
    QString getNomFournisseur();
    QString getAdresse();
    QString getTelephoneFournisseur();
    QString getEmailFournisseur();
    QString getDateContrat();

    void setIdFournisseur(int);
    void setNomFournisseur(QString);
    void setAdresse(QString);
    void setTelephoneFournisseur(QString);
    void setEmailFournisseur(QString);
    void setDateContrat(QString);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString date_contrat);
    void printPDF();
    QSqlQueryModel* afficherTriParId();
    QSqlQueryModel* trierParNom();




private:
    int id_fournisseur;
    QString nom_fournisseur;
    QString adresse;
    QString telephone_fournisseur;
    QString email_fournisseur;
    QString date_contrat;
};

#endif // FOURNISSEUR_H
