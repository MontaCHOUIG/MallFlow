#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "evenement.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QIntValidator>
#include <QComboBox>
#include <QFileDialog>
#include <QTextStream>
#include <QPrinter>
#include <QPainter>
#include <algorithm>
#include <QDesktopServices>
#include <QUrl>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Sp_Line_ID->setValidator(new QIntValidator(1, 1000000, this));

    connect(ui->Sp_Combo_ID, &QComboBox::currentTextChanged,
            this, &MainWindow::on_Sp_Combo_ID_currentIndexChanged);

    connect(ui->Sp_Button_Recherche, &QPushButton::clicked,
            this, &MainWindow::on_Sp_Button_Recherche_clicked);
    connect(ui->Sp_Button_Tri_Titre, &QPushButton::clicked,
            this, &MainWindow::on_Sp_Button_Tri_Titre_clicked);
    connect(ui->Sp_Button_Tri_Lieu, &QPushButton::clicked,
            this, &MainWindow::on_Sp_Button_Tri_Lieu_clicked);
    connect(ui->Sp_Button_Tri_ID, &QPushButton::clicked,
            this, &MainWindow::on_Sp_Button_Tri_ID_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Sp_Button_Ajouter_clicked() {
    int id = ui->Sp_Line_ID->text().toInt();
    QString titre = ui->Sp_Line_Titre->text().trimmed();
    QString lieu = ui->Sp_Line_Lieu->text().trimmed();
    QDate dateDebut = ui->dateEdit->date();
    QDate dateFin = ui->dateEdit_2->date();
    QString typeParticipants = ui->Sp_Line_Type->text().trimmed();

    if (titre.isEmpty() || lieu.isEmpty() || typeParticipants.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Dates invalides", "La date de fin doit être postérieure ou égale à la date de début.");
        return;
    }
    for (const Evenement &e : evenements) {
        if (e.getId() == id) {
            QMessageBox::warning(this, "ID existant", "Un événement avec cet ID existe déjà.");
            return;
        }
    }

    evenements.append(Evenement(id, titre, lieu, dateDebut, dateFin, typeParticipants));
    afficherEvenements();

    ui->Sp_Line_ID->clear();
    ui->Sp_Line_Titre->clear();
    ui->Sp_Line_Lieu->clear();
    ui->Sp_Line_Type->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->tabWidget->setCurrentIndex(1);
    QMessageBox::information(this, "Succès", "Événement ajouté avec succès.");
}

void MainWindow::on_Sp_Combo_ID_currentIndexChanged(const QString &idStr) {
    if (idStr.isEmpty()) return;
    int id = idStr.toInt();
    for (const Evenement &e : evenements) {
        if (e.getId() == id) {
            ui->Sp_Line_ID->setText(idStr);
            ui->Sp_Line_Titre->setText(e.getTitre());
            ui->Sp_Line_Lieu->setText(e.getLieu());
            ui->dateEdit->setDate(e.getDateDebut());
            ui->dateEdit_2->setDate(e.getDateFin());
            ui->Sp_Line_Type->setText(e.getTypeParticipants());
            break;
        }
    }
}

void MainWindow::on_Sp_Button_Modifier_clicked() {
    int id = ui->Sp_Combo_ID->currentText().toInt();
    QString titre = ui->Sp_Line_Titre->text().trimmed();
    QString lieu = ui->Sp_Line_Lieu->text().trimmed();
    QDate dateDebut = ui->dateEdit->date();
    QDate dateFin = ui->dateEdit_2->date();
    QString typeParticipants = ui->Sp_Line_Type->text().trimmed();

    if (titre.isEmpty() || lieu.isEmpty() || typeParticipants.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez remplir tous les champs.");
        return;
    }

    for (Evenement &e : evenements) {
        if (e.getId() == id) {
            e.setTitre(titre);
            e.setLieu(lieu);
            e.setDateDebut(dateDebut);
            e.setDateFin(dateFin);
            e.setTypeParticipants(typeParticipants);
            afficherEvenements();
            QMessageBox::information(this, "Succès", "Événement modifié avec succès.");
            return;
        }
    }
    QMessageBox::warning(this, "Erreur", "Aucun événement trouvé.");
}

void MainWindow::on_Sp_Button_Supprimer_clicked() {
    int id = ui->Sp_Combo_ID->currentText().toInt();
    for (int i = 0; i < evenements.size(); ++i) {
        if (evenements[i].getId() == id) {
            evenements.removeAt(i);
            afficherEvenements();
            QMessageBox::information(this, "Succès", "Événement supprimé avec succès.");
            return;
        }
    }
    QMessageBox::warning(this, "Erreur", "Événement non trouvé.");
}

void MainWindow::afficherEvenements(const QList<Evenement> &liste) {
    QList<Evenement> aAfficher = liste.isEmpty() ? evenements : liste;
    auto *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"ID", "Titre", "Lieu", "Date Début", "Date Fin", "Participants"});

    for (const Evenement &e : aAfficher) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(e.getId()))
            << new QStandardItem(e.getTitre())
            << new QStandardItem(e.getLieu())
            << new QStandardItem(e.getDateDebut().toString("yyyy-MM-dd"))
            << new QStandardItem(e.getDateFin().toString("yyyy-MM-dd"))
            << new QStandardItem(e.getTypeParticipants());
        model->appendRow(row);
    }
    ui->Sp_TableView_Res->setModel(model);

    ui->Sp_Combo_ID->clear();
    for (const Evenement &e : evenements)
        ui->Sp_Combo_ID->addItem(QString::number(e.getId()));
}

void MainWindow::on_Sp_Button_Recherche_clicked() {
    QString titreRecherche = ui->Sp_Line_Recherche->text().trimmed();
    QList<Evenement> resultats;
    for (const Evenement &e : evenements) {
        if (e.getTitre().contains(titreRecherche, Qt::CaseInsensitive))
            resultats.append(e);
    }
    afficherEvenements(resultats);
}

void MainWindow::on_Sp_Button_Tri_Titre_clicked() {
    std::sort(evenements.begin(), evenements.end(), [](const Evenement &a, const Evenement &b) {
        return a.getTitre().toLower() < b.getTitre().toLower();
    });
    afficherEvenements();
}

void MainWindow::on_Sp_Button_Tri_Lieu_clicked() {
    std::sort(evenements.begin(), evenements.end(), [](const Evenement &a, const Evenement &b) {
        return a.getLieu().toLower() < b.getLieu().toLower();
    });
    afficherEvenements();
}

void MainWindow::on_Sp_Button_Tri_ID_clicked() {
    std::sort(evenements.begin(), evenements.end(), [](const Evenement &a, const Evenement &b) {
        return a.getId() < b.getId();
    });
    afficherEvenements();
}

void MainWindow::on_Sp_Button_PDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    QString html = "<h2>Liste des événements</h2><table border='1' cellspacing='0' cellpadding='2'>"
                   "<tr><th>ID</th><th>Titre</th><th>Lieu</th><th>Date Début</th><th>Date Fin</th><th>Type</th></tr>";

    for (const Evenement &e : evenements) {
        html += "<tr><td>" + QString::number(e.getId()) + "</td>"
                + "<td>" + e.getTitre() + "</td>"
                + "<td>" + e.getLieu() + "</td>"
                + "<td>" + e.getDateDebut().toString("yyyy-MM-dd") + "</td>"
                + "<td>" + e.getDateFin().toString("yyyy-MM-dd") + "</td>"
                + "<td>" + e.getTypeParticipants() + "</td></tr>";
    }
    html += "</table>";

    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "PDF généré avec succès !");
    // Optionnel : ouvrir automatiquement le fichier
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}
