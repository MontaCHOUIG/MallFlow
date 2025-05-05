#include "fournisseur.h"
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <utility>

fournisseur::fournisseur()
{
    id_fournisseur = 0;
    nom_fournisseur = " ";
    adresse = " ";
    telephone_fournisseur = " ";
    email_fournisseur = " ";
    type_fournisseur = " ";
}

fournisseur::fournisseur(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString type_fournisseur)
{
    this->id_fournisseur = id_fournisseur;
    this->nom_fournisseur = nom_fournisseur;
    this->adresse = adresse;
    this->telephone_fournisseur = telephone_fournisseur;
    this->email_fournisseur = email_fournisseur;
    this->type_fournisseur = type_fournisseur;
}

int fournisseur::getIdFournisseur() { return id_fournisseur; }
QString fournisseur::getNomFournisseur() { return nom_fournisseur; }
QString fournisseur::getAdresse() { return adresse; }
QString fournisseur::getTelephoneFournisseur() { return telephone_fournisseur; }
QString fournisseur::getEmailFournisseur() { return email_fournisseur; }
QString fournisseur::getTypeFournisseur() { return type_fournisseur; }

void fournisseur::setIdFournisseur(int id_fournisseur) { this->id_fournisseur = id_fournisseur; }
void fournisseur::setNomFournisseur(QString nom_fournisseur) { this->nom_fournisseur = nom_fournisseur; }
void fournisseur::setAdresse(QString adresse) { this->adresse = adresse; }
void fournisseur::setTelephoneFournisseur(QString telephone_fournisseur) { this->telephone_fournisseur = telephone_fournisseur; }
void fournisseur::setEmailFournisseur(QString email_fournisseur) { this->email_fournisseur = email_fournisseur; }
void fournisseur::setTypeFournisseur(QString type_fournisseur) { this->type_fournisseur = type_fournisseur; }

bool fournisseur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO FOURNISSEURS (id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, type_fournisseur) "
                  "VALUES (:id_fournisseur, :nom_fournisseur, :adresse, :telephone_fournisseur, :email_fournisseur, :type_fournisseur)");
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":nom_fournisseur", nom_fournisseur);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone_fournisseur", telephone_fournisseur);
    query.bindValue(":email_fournisseur", email_fournisseur);
    query.bindValue(":type_fournisseur", type_fournisseur);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS");
    return model;
}

bool fournisseur::supprimer(int id_fournisseur)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FOURNISSEURS WHERE id_fournisseur = :id_fournisseur");
    query.bindValue(":id_fournisseur", id_fournisseur);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

bool fournisseur::modifier(int id_fournisseur, QString nom_fournisseur, QString adresse, QString telephone_fournisseur, QString email_fournisseur, QString type_fournisseur)
{
    QSqlQuery query;
    query.prepare("UPDATE FOURNISSEURS SET nom_fournisseur=:nom_fournisseur, adresse=:adresse, telephone_fournisseur=:telephone_fournisseur, email_fournisseur=:email_fournisseur, type_fournisseur=:type_fournisseur WHERE id_fournisseur=:id_fournisseur");
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":nom_fournisseur", nom_fournisseur);
    query.bindValue(":adresse", adresse);
    query.bindValue(":telephone_fournisseur", telephone_fournisseur);
    query.bindValue(":email_fournisseur", email_fournisseur);
    query.bindValue(":type_fournisseur", type_fournisseur);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

void fournisseur::printPDF()
{
    QString filePath = QFileDialog::getSaveFileName(
        nullptr,
        "Enregistrer le PDF",
        "",
        "Fichiers PDF (*.pdf)"
        );

    if (filePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Annulé", "Aucun emplacement sélectionné.");
        return;
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdf(filePath);
    QPainter painter(&pdf);
    int i = 4000;

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(2000, 1500, "Liste des fournisseurs");

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 15));
    painter.drawRect(100, 100, 8500, 2500);
    painter.drawRect(0, 3000, 9600, 500);

    painter.setFont(QFont("Arial", 10));
    painter.drawText(200, 3300, "ID");
    painter.drawText(800, 3300, "Nom");
    painter.drawText(2000, 3300, "Adresse");
    painter.drawText(3400, 3300, "Téléphone");
    painter.drawText(5000, 3300, "Email");
    painter.drawText(7000, 3300, "Type Fournisseur");

    QSqlQuery query;
    query.prepare("SELECT * FROM FOURNISSEURS");
    query.exec();

    while (query.next())
    {
        painter.drawText(200, i, query.value(0).toString());
        painter.drawText(800, i, query.value(1).toString());
        painter.drawText(2000, i, query.value(2).toString());
        painter.drawText(3400, i, query.value(3).toString());
        painter.drawText(5000, i, query.value(4).toString());
        painter.drawText(7000, i, query.value(5).toString());
        i += 500;
    }

    QMessageBox::information(nullptr, "PDF généré", "Le fichier PDF a été créé avec succès.");
}

QSqlQueryModel* fournisseur::afficherTriParId()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS ORDER BY id_fournisseur ASC");
    return model;
}

QSqlQueryModel* fournisseur::trierParNom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEURS ORDER BY nom_fournisseur ASC");
    return model;
}

QSqlQueryModel* fournisseur::rechercherParNom(const QString& nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM FOURNISSEURS WHERE nom_fournisseur LIKE :nom");
    query.bindValue(":nom", "%" + nom + "%");
    query.exec();
    model->setQuery(std::move(query));
    return model;
}

QMap<QString, int> fournisseur::getStatsByType()
{
    QMap<QString, int> stats;
    QSqlQuery query("SELECT type_fournisseur, COUNT(*) FROM FOURNISSEURS GROUP BY type_fournisseur");
    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        stats[type] = count;
    }
    return stats;
}

bool fournisseur::importerFournisseurs(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Erreur: Impossible d'ouvrir le fichier" << filePath;
        return false;
    }

    QTextStream in(&file);
    bool firstLine = true;
    int successCount = 0;
    int errorCount = 0;
    QSqlQuery checkQuery;
    QSqlQuery insertQuery;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (firstLine) {
            firstLine = false;
            continue;
        }

        QStringList fields = line.split(",");
        if (fields.size() != 6) {
            qDebug() << "Ligne invalide (nombre de champs incorrect):" << line;
            errorCount++;
            continue;
        }

        bool ok;
        int id_fournisseur = fields[0].toInt(&ok);
        if (!ok) {
            qDebug() << "ID invalide:" << fields[0];
            errorCount++;
            continue;
        }

        QString nom_fournisseur = fields[1].trimmed();
        QString adresse = fields[2].trimmed();
        QString telephone_fournisseur = fields[3].trimmed();
        QString email_fournisseur = fields[4].trimmed();
        QString type_fournisseur = fields[5].trimmed();

        if (nom_fournisseur.isEmpty() || adresse.isEmpty() || telephone_fournisseur.isEmpty() ||
            email_fournisseur.isEmpty() || type_fournisseur.isEmpty()) {
            qDebug() << "Champs vides détectés pour ID:" << id_fournisseur;
            errorCount++;
            continue;
        }

        if (!email_fournisseur.endsWith("@gmail.com")) {
            qDebug() << "Email invalide pour ID:" << id_fournisseur << ":" << email_fournisseur;
            errorCount++;
            continue;
        }

        if (type_fournisseur != "stratégiques" && type_fournisseur != "sous-traitants") {
            qDebug() << "Type invalide pour ID:" << id_fournisseur << ":" << type_fournisseur;
            errorCount++;
            continue;
        }

        checkQuery.prepare("SELECT COUNT(*) FROM FOURNISSEURS WHERE id_fournisseur = :id_fournisseur");
        checkQuery.bindValue(":id_fournisseur", id_fournisseur);
        if (!checkQuery.exec()) {
            qDebug() << "Erreur lors de la vérification de l'ID:" << checkQuery.lastError().text();
            errorCount++;
            continue;
        }
        checkQuery.first();
        if (checkQuery.value(0).toInt() > 0) {
            qDebug() << "ID déjà existant:" << id_fournisseur;
            errorCount++;
            continue;
        }

        insertQuery.prepare("INSERT INTO FOURNISSEURS (id_fournisseur, nom_fournisseur, adresse, telephone_fournisseur, email_fournisseur, type_fournisseur) "
                            "VALUES (:id_fournisseur, :nom_fournisseur, :adresse, :telephone_fournisseur, :email_fournisseur, :type_fournisseur)");
        insertQuery.bindValue(":id_fournisseur", id_fournisseur);
        insertQuery.bindValue(":nom_fournisseur", nom_fournisseur);
        insertQuery.bindValue(":adresse", adresse);
        insertQuery.bindValue(":telephone_fournisseur", telephone_fournisseur);
        insertQuery.bindValue(":email_fournisseur", email_fournisseur);
        insertQuery.bindValue(":type_fournisseur", type_fournisseur);

        if (!insertQuery.exec()) {
            qDebug() << "Erreur lors de l'insertion pour ID:" << id_fournisseur << ":" << insertQuery.lastError().text();
            errorCount++;
        } else {
            qDebug() << "Fournisseur inséré avec succès, ID:" << id_fournisseur;
            successCount++;
        }
    }

    file.close();
    qDebug() << "Importation terminée:" << successCount << "fournisseurs ajoutés," << errorCount << "erreurs.";
    return successCount > 0;
}

void fournisseur::logAccess(int id_fournisseur, const QString &rfid_uid, bool autorise)
{
    // Vérifier si l'UID est valide et ne dépasse pas 20 caractères
    if (rfid_uid.length() > 20) {
        qDebug() << "UID trop long pour la base de données:" << rfid_uid;
        return;
    }

    // Pour les accès non autorisés, insérer NULL pour ID_FOURNISSEUR
    QVariant id_fournisseur_value = (id_fournisseur == 0) ? QVariant(QVariant::Int) : id_fournisseur;

    // Vérifier si l'ID_FOURNISSEUR existe dans FOURNISSEURS (sauf si id_fournisseur est 0)
    if (id_fournisseur != 0) {
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM FOURNISSEURS WHERE ID_FOURNISSEUR = :id_fournisseur");
        checkQuery.bindValue(":id_fournisseur", id_fournisseur);
        if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
            qDebug() << "ID_FOURNISSEUR non trouvé dans FOURNISSEURS:" << id_fournisseur;
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO ACCES_FOURNISSEURS (ID_ACCES, ID_FOURNISSEUR, RFID_UID, AUTORISE) "
                  "VALUES (SEQ_ACCES_FOURNISSEURS.NEXTVAL, :id_fournisseur, :rfid_uid, :autorise)");
    query.bindValue(":id_fournisseur", id_fournisseur_value);
    query.bindValue(":rfid_uid", rfid_uid);
    query.bindValue(":autorise", autorise ? "Y" : "N");

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'enregistrement de l'accès:" << query.lastError().text();
    }
}
