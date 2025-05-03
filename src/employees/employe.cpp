#include "employe.h"
#include <QDebug>
#include <QSqlError>


Employe::Employe() {}

Employe::Employe(int id, const QString& nom, const QString& poste, const QString& email,
                 const QString& role, double salaire, const QString& mdp, int secure_auth)
    : id_employe(id), nom(nom), poste(poste), email(email), role(role), salaire(salaire), mdp(mdp),secure_auth(secure_auth) {}

// Getters
int Employe::getIdEmploye() const { return id_employe; }
QString Employe::getNom() const { return nom; }
QString Employe::getPoste() const { return poste; }
QString Employe::getEmail() const { return email; }
QString Employe::getRole() const { return role; }
double Employe::getSalaire() const { return salaire; }
QString Employe::getMdp() const { return mdp; }
int Employe::getSecureAuth() const {return secure_auth;}

// Setters
void Employe::setIdEmploye(int id) { id_employe = id; }
void Employe::setNom(const QString& nom) { this->nom = nom; }
void Employe::setPoste(const QString& poste) { this->poste = poste; }
void Employe::setEmail(const QString& email) { this->email = email; }
void Employe::setRole(const QString& role) { this->role = role; }
void Employe::setSalaire(double salaire) { this->salaire = salaire; }
void Employe::setMdp(const QString& mdp) { this->mdp = mdp; }
void Employe::setSecureAuth(int secure_auth){this->secure_auth = secure_auth; }

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
    query.prepare("INSERT INTO EMPLOYES (ID_EMPLOYE, NOM, POSTE, EMAIL, ROLE, SALAIRE, MOT_DE_PASSE, SECURE_AUTH) "
                  "VALUES (:id, :nom, :poste, :email, :role, :salaire, :mdp, :secure_auth)");
    query.bindValue(":id", id_employe);
    query.bindValue(":nom", nom);
    query.bindValue(":poste", poste);
    query.bindValue(":email", email);
    query.bindValue(":role", role);
    query.bindValue(":salaire", salaire);
    query.bindValue(":mdp", hashPassword(mdp));
    query.bindValue(":secure_auth" , secure_auth);

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

void Employe::setSecurityQuestions(const QString &q1, const QString &a1,
                                   const QString &q2, const QString &a2) {
    QSqlQuery query;
    query.prepare("INSERT INTO SECURITY_QUESTIONS (EMPLOYEE_ID, QUESTION_1, ANSWER_1, QUESTION_2, ANSWER_2) "
                  "VALUES (:id, :q1, :a1, :q2, :a2)");
    query.bindValue(":id", this->id_employe);  // Make sure you have the employee's ID set
    query.bindValue(":q1", q1);
    query.bindValue(":a1", a1);
    query.bindValue(":q2", q2);
    query.bindValue(":a2", a2);
    query.exec();  // You can check for success with `if (!query.exec())` if needed
}

void Employe::saveAuthenticatedUser(const QString &email) {
    QSqlQuery query;
    query.prepare("SELECT ROLE, POSTE FROM EMPLOYES WHERE EMAIL = :email");
    query.bindValue(":email", email);

    QString role, poste;
    if (query.exec() && query.next()) {
        role = query.value("ROLE").toString();
        poste = query.value("POSTE").toString();
    } else {
        qDebug() << "Failed to fetch role and poste:" << query.lastError().text();
        return;
    }

    QSettings settings("MallFlow", "Auth");
    settings.setValue("authenticatedUser", email);
    settings.setValue("userRole", role);
    settings.setValue("userPoste", poste);
}

QString Employe::getAuthenticatedUser() {
    QSettings settings("MallFlow", "Auth");
    return settings.value("authenticatedUser", "").toString();
}

QString Employe::getAuthenticatedUserRole() {
    QSettings settings("MallFlow", "Auth");
    return settings.value("userRole", "").toString();
}

QString Employe::getAuthenticatedUserPoste() {
    QSettings settings("MallFlow", "Auth");
    return settings.value("userPoste", "").toString();
}

void Employe::logoutUser() {
    QSettings settings("MallFlow", "Auth");
    settings.remove("authenticatedUser");
    settings.remove("userRole");
    settings.remove("userPoste");
}

bool Employe::hasWindowAccess(const QString& windowName) {
    QString role = getAuthenticatedUserRole();
    QString poste = getAuthenticatedUserPoste();

    // Admin has access to all windows
    if (role == "admin") {
        return true;
    }

    // Define access rules based on role and poste
    if (windowName == "employees") {
        return role == "admin" || poste == "RH";
    }
    else if (windowName == "sponsors") {
        return role == "admin" || poste == "Sponsoring";
    }
    else if (windowName == "services") {
        return role == "admin" || poste == "Service";
    }
    else if (windowName == "evenements") {
        return role == "admin" || poste == "Evenement";
    }
    else if (windowName == "fournisseurs") {
        return role == "admin" || poste == "Achat";
    }
    else if (windowName == "magasins") {
        return role == "admin" || poste == "Commercial";
    }

    return false;
}
