#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>

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

    // Getters
    int getIdEmploye() const;
    QString getNom() const;
    QString getPoste() const;
    QString getEmail() const;
    QString getRole() const;
    double getSalaire() const;
    QString getMdp() const;

    // Setters
    void setIdEmploye(int id);
    void setNom(const QString& nom);
    void setPoste(const QString& poste);
    void setEmail(const QString& email);
    void setRole(const QString& role);
    void setSalaire(double salaire);
    void setMdp(const QString& mdp);
};

#endif // EMPLOYE_H
