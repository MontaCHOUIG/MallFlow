#include "evenement.h"

Evenement::Evenement(int id, const QString& titre, const QString& lieu, const QDate& dateDebut, const QDate& dateFin, const QString& typeParticipants)
    : id(id), titre(titre), lieu(lieu), dateDebut(dateDebut), dateFin(dateFin), typeParticipants(typeParticipants) {}

int Evenement::getId() const {
    return id;
}

QString Evenement::getTitre() const {
    return titre;
}

QString Evenement::getLieu() const {
    return lieu;
}

QDate Evenement::getDateDebut() const {
    return dateDebut;
}

QDate Evenement::getDateFin() const {
    return dateFin;
}

QString Evenement::getTypeParticipants() const {
    return typeParticipants;
}

void Evenement::setTitre(const QString& titre) {
    this->titre = titre;
}

void Evenement::setLieu(const QString& lieu) {
    this->lieu = lieu;
}

void Evenement::setDateDebut(const QDate& dateDebut) {
    this->dateDebut = dateDebut;
}

void Evenement::setDateFin(const QDate& dateFin) {
    this->dateFin = dateFin;
}

void Evenement::setTypeParticipants(const QString& typeParticipants) {
    this->typeParticipants = typeParticipants;
}
