#ifndef MAGASIN_H
#define MAGASIN_H

#include <QString>

class Magasin {
public:
    Magasin();
    Magasin(int id, const QString &nom, const QString &telephone,
            const QString &email, double prix, const QString &reference);

    int get_ID_MAGASIN() const;
    QString get_NOM_MAGASIN() const;
    QString get_TELEPHONE_LOCATAIRE() const;
    QString get_EMAIL_DE_MAGASIN() const;
    double get_PRIX_DE_LOCATION() const;
    QString get_REFERENCE() const;

    void set_NOM_MAGASIN(const QString &nom);
    void set_TELEPHONE_LOCATAIRE(const QString &telephone);
    void set_EMAIL_DE_MAGASIN(const QString &email);
    void set_PRIX_DE_LOCATION(double prix);
    void set_REFERENCE(const QString &reference);

    QString to_string() const;

    static Magasin magasinVide(); // constructeur "vide" pour la recherche

private:
    int id_magasin;
    QString nom_magasin;
    QString telephone_locataire;
    QString email_de_magasin;
    double prix_de_location;
    QString reference;
};

#endif // MAGASIN_H
