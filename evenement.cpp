#include "evenement.h"

Evenement::Evenement(int id, const QString& titre, const QDate& dateDebut, const QDate& dateFin,
                     const QString& lieu, const QString& typeParticipants,
                     int idEmploye, int idSponsor)
    : id_evenement(id), titre(titre), date_debut(dateDebut), date_fin(dateFin),
    lieu(lieu), type_participants(typeParticipants), id_employe(idEmploye), id_sponsor(idSponsor) {}

// Getters
int Evenement::getId() const { return id_evenement; }
QString Evenement::getTitre() const { return titre; }
QDate Evenement::getDateDebut() const { return date_debut; }
QDate Evenement::getDateFin() const { return date_fin; }
QString Evenement::getLieu() const { return lieu; }
QString Evenement::getTypeParticipants() const { return type_participants; }
int Evenement::getIdEmploye() const { return id_employe; }
int Evenement::getIdSponsor() const { return id_sponsor; }

// Setters
void Evenement::setTitre(const QString& titre) { this->titre = titre; }
void Evenement::setDateDebut(const QDate& dateDebut) { this->date_debut = dateDebut; }
void Evenement::setDateFin(const QDate& dateFin) { this->date_fin = dateFin; }
void Evenement::setLieu(const QString& lieu) { this->lieu = lieu; }
void Evenement::setTypeParticipants(const QString& typeParticipants) { this->type_participants = typeParticipants; }
void Evenement::setIdEmploye(int idEmploye) { this->id_employe = idEmploye; }
void Evenement::setIdSponsor(int idSponsor) { this->id_sponsor = idSponsor; }
