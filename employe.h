#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QCryptographicHash>
#include <QSettings>

class Employe {
private:
    int id_employe;
    QString nom;
    QString poste;
    QString email;
    QString role;
    double salaire;
    QString mdp;

public:
    // Constructors
    Employe();
    Employe(int id, const QString& nom, const QString& poste, const QString& email,
            const QString& role, double salaire, const QString& mdp);

    // Getters & Setters
    int getIdEmploye() const;
    void setIdEmploye(int id);
    QString getNom() const;
    void setNom(const QString& nom);
    QString getPoste() const;
    void setPoste(const QString& poste);
    QString getEmail() const;
    void setEmail(const QString& email);
    QString getRole() const;
    void setRole(const QString& role);
    double getSalaire() const;
    void setSalaire(double salaire);
    QString getMdp() const;
    void setMdp(const QString& mdp);

    // CRUD Operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();

    QString hashPassword(const QString &password);
    bool authenticateUser(const QString &email, const QString &password);
    void saveAuthenticatedUser(const QString &email);
    QString getAuthenticatedUser();
    void logoutUser();


};

#endif // EMPLOYE_H
