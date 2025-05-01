#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QString>
#include <QDate>

class Evenement {
public:
    Evenement(int id, const QString& titre, const QDate& dateDebut, const QDate& dateFin,
              const QString& lieu, const QString& typeParticipants, int idEmploye, int idSponsor);

    int getId() const;
    QString getTitre() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getLieu() const;
    QString getTypeParticipants() const;
    int getIdEmploye() const;
    int getIdSponsor() const;

    void setTitre(const QString& titre);
    void setDateDebut(const QDate& dateDebut);
    void setDateFin(const QDate& dateFin);
    void setLieu(const QString& lieu);
    void setTypeParticipants(const QString& typeParticipants);
    void setIdEmploye(int idEmploye);
    void setIdSponsor(int idSponsor);

private:
    int id_evenement;
    QString titre;
    QDate date_debut;
    QDate date_fin;
    QString lieu;
    QString type_participants;
    int id_employe;
    int id_sponsor;
};

#endif // EVENEMENT_H