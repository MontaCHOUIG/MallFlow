#include "employe.h"
#include <QDebug>
#include <QSqlError>


Employe::Employe() {}

Employe::Employe(int id, const QString& nom, const QString& poste, const QString& email,
                 const QString& role, double salaire, const QString& mdp)
    : id_employe(id), nom(nom), poste(poste), email(email), role(role), salaire(salaire), mdp(mdp) {}

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

QString Employe::hashPassword(const QString &password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}


bool Employe::authenticateUser(const QString &email, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT MOT_DE_PASSE FROM Employes WHERE email = :email");
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Database query failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedHash = query.value(0).toString();
        QString enteredHash = hashPassword(password);
        qDebug() << "Stored Hash:" << storedHash;
        qDebug() << "Entered Hash:" << enteredHash;

        return storedHash == enteredHash;
    }

    qDebug() << "User not found or no password stored.";
    return false;
}


// ADD Employe
bool Employe::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYES (ID_EMPLOYE, NOM, POSTE, EMAIL, ROLE, SALAIRE, MOT_DE_PASSE) "
                  "VALUES (:id, :nom, :poste, :email, :role, :salaire, :mdp)");
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":role", role);
    query.bindValue(":salaire", salaire);
    query.bindValue(":mdp", hashPassword(mdp));  // Changed "mdp" to match the correct column "MOT_DE_PASSE"

    if (!query.exec()) {
        //qDebug() << "Failed to add employee:" << query.lastError().text();
        return false;
    }
    return true;
}

// READ Employees
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT ID_EMPLOYE, NOM, POSTE, EMAIL, ROLE, SALAIRE, MOT_DE_PASSE FROM EMPLOYES");

    if (!query.exec()) {
        qDebug() << "Failed to fetch employees:" << query.lastError().text();
    } else {
        model->setQuery(query);
    }
    return model;
}

// DELETE Employee
bool Employe::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYES WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to delete employee:" << query.lastError().text();
        return false;
    }
    return true;
}

// UPDATE Employee
bool Employe::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYES SET NOM = :nom, POSTE = :poste, EMAIL = :email, "
                  "ROLE = :role, SALAIRE = :salaire, MOT_DE_PASSE = :mdp WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":role", role);
    query.bindValue(":salaire", salaire);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Failed to update employee:" << query.lastError().text();
        return false;
    }
    return true;
}


void Employe::saveAuthenticatedUser(const QString &email) {
    QSettings settings("MallFlow", "Auth");
    settings.setValue("authenticatedUser", email);
}

QString Employe::getAuthenticatedUser() {
    QSettings settings("MallFlow", "Auth");
    return settings.value("authenticatedUser", "").toString();
}


void Employe::logoutUser() {
    QSettings settings("MallFlow", "Auth");
    settings.remove("authenticatedUser");
}
