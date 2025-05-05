#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQueryModel>

class fournisseur
{
public:
    fournisseur();
    fournisseur(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString type_fournisseur);

    int getIdFournisseur();
    QString getNomFournisseur();
    QString getAdresse();
    QString getTelephoneFournisseur();
    QString getEmailFournisseur();
    QString getTypeFournisseur();

    void setIdFournisseur(int id_fournisseur);
    void setNomFournisseur(QString nom_fournisseur);
    void setAdresse(QString adresse);
    void setTelephoneFournisseur(QString telephone_fournisseur);
    void setEmailFournisseur(QString email_fournisseur);
    void setTypeFournisseur(QString type_fournisseur);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id_fournisseur);
    bool modifier(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString type_fournisseur);
    void printPDF();
    QSqlQueryModel* afficherTriParId();
    QSqlQueryModel* trierParNom();
    QSqlQueryModel* rechercherParNom(const QString& nom);
    QMap<QString, int> getStatsByType();
    bool importerFournisseurs(const QString& filePath);
    void logAccess(int id_fournisseur, const QString &rfid_uid, bool autorise);

private:
    int id_fournisseur;
    QString nom_fournisseur;
    QString adresse;
    QString telephone_fournisseur;
    QString email_fournisseur;
    QString type_fournisseur;
};

#endif // FOURNISSEUR_H
