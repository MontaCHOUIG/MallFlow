#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QString>
#include <QDate>

class Evenement {
public:
    Evenement(int id, const QString& titre, const QString& lieu, const QDate& dateDebut, const QDate& dateFin, const QString& typeParticipants);

    int getId() const;
    QString getTitre() const;
    QString getLieu() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getTypeParticipants() const;

    void setTitre(const QString& titre);
    void setLieu(const QString& lieu);
    void setDateDebut(const QDate& dateDebut);
    void setDateFin(const QDate& dateFin);
    void setTypeParticipants(const QString& typeParticipants);

private:
    int id;
    QString titre;
    QString lieu;
    QDate dateDebut;
    QDate dateFin;
    QString typeParticipants; // Nouvel attribut
};

#endif // EVENEMENT_H
