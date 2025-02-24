#include "Employe.h"

// Default constructor
Employe::Employe() : id_employe(0), salaire(0.0) {}

// Parameterized constructor
Employe::Employe(int id, const QString& nom, const QString& poste, const QString& email,
                 const QString& role, double salaire, const QString& mdp)
    : id_employe(id), nom(nom), poste(poste), email(email), role(role),
    salaire(salaire), mdp(mdp) {}

// Getters
int Employe::getIdEmploye() const { return id_employe; }
QString Employe::getNom() const { return nom; }
QString Employe::getPoste() const { return poste; }
QString Employe::getEmail() const { return email; }
QString Employe::getRole() const { return role; }
double Employe::getSalaire() const { return salaire; }
QString Employe::getMdp() const { return mdp; }

// Setters
void Employe::setIdEmploye(int id) { id_employe = id; }
void Employe::setNom(const QString& nom) { this->nom = nom; }
void Employe::setPoste(const QString& poste) { this->poste = poste; }
void Employe::setEmail(const QString& email) { this->email = email; }
void Employe::setRole(const QString& role) { this->role = role; }
void Employe::setSalaire(double salaire) { this->salaire = salaire; }
void Employe::setMdp(const QString& mdp) { this->mdp = mdp; }
