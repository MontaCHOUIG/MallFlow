#include "magasin.h"

Magasin::Magasin() : id_magasin(0), prix_de_location(0.0) {}

Magasin::Magasin(int id, const QString &nom, const QString &telephone,
                 const QString &email, double prix, const QString &reference)
    : id_magasin(id), nom_magasin(nom), telephone_locataire(telephone),
    email_de_magasin(email), prix_de_location(prix), reference(reference) {}

int Magasin::get_ID_MAGASIN() const {
    return id_magasin;
}

QString Magasin::get_NOM_MAGASIN() const {
    return nom_magasin;
}

QString Magasin::get_TELEPHONE_LOCATAIRE() const {
    return telephone_locataire;
}

QString Magasin::get_EMAIL_DE_MAGASIN() const {
    return email_de_magasin;
}

double Magasin::get_PRIX_DE_LOCATION() const {
    return prix_de_location;
}

QString Magasin::get_REFERENCE() const {
    return reference;
}

void Magasin::set_NOM_MAGASIN(const QString &nom) {
    nom_magasin = nom;
}

void Magasin::set_TELEPHONE_LOCATAIRE(const QString &telephone) {
    telephone_locataire = telephone;
}

void Magasin::set_EMAIL_DE_MAGASIN(const QString &email) {
    email_de_magasin = email;
}

void Magasin::set_PRIX_DE_LOCATION(double prix) {
    prix_de_location = prix;
}

void Magasin::set_REFERENCE(const QString &ref) {
    reference = ref;
}

QString Magasin::to_string() const {
    return QString("ID: %1, Nom: %2, Téléphone: %3, Email: %4, Prix: %5, Référence: %6")
        .arg(id_magasin)
        .arg(nom_magasin)
        .arg(telephone_locataire)
        .arg(email_de_magasin)
        .arg(prix_de_location)
        .arg(reference);
}

Magasin Magasin::magasinVide() {
    return Magasin(0, "", "", "", 0.0, "");
}
