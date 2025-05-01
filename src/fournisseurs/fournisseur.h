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

    QMap<QString, int> getStatsByType();
    fournisseur();
    fournisseur(int, QString, QString, QString, QString, QString);

    int getIdFournisseur();
    QString getNomFournisseur();
    QString getAdresse();
    QString getTelephoneFournisseur();
    QString getEmailFournisseur();
    QString getTypeFournisseur();

    void setIdFournisseur(int);
    void setNomFournisseur(QString);
    void setAdresse(QString);
    void setTelephoneFournisseur(QString);
    void setEmailFournisseur(QString);
    void setTypeFournisseur(QString);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString type_fournisseur);
    void printPDF();
    QSqlQueryModel* afficherTriParId();
    QSqlQueryModel* trierParNom();
    QSqlQueryModel* rechercherParNom(const QString& nom);
    bool importerFournisseurs(const QString& filePath);





private:
    int id_fournisseur;
    QString nom_fournisseur;
    QString adresse;
    QString telephone_fournisseur;
    QString email_fournisseur;
    QString type_fournisseur;
};

#endif // FOURNISSEUR_H
