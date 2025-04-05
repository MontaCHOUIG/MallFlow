#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QTextDocument>
#include <QFileDialog>

// Constructeur de la classe MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) // Initialisation de l'interface utilisateur
{
    ui->setupUi(this); // Configuration de l'interface utilisateur
    setWindowTitle("MallFlow"); // Définition du titre de la fenêtre


    ui->Sp_Line_ID->setValidator(new QIntValidator(0, 999999, this));
    ui->Sp_Line_Numtel->setValidator(new QIntValidator(0, 999999999, this));

    ui->Sp_TableView_Res->setModel(S.afficher()); // Affiche les sponsors dans la table
    ui->Sp_TableView_Res->setColumnWidth(3, 250); // Ajuste la largeur de la colonne Email
    ui->Sp_TableView_Res->setColumnWidth(2, 250); // Ajuste la largeur de la colonne Numéro de téléphone
    ui->Sp_Combo_IDs->setModel(S.afficher_id()); // Remplit la combobox avec les ID des sponsors
}

// Destructeur de la classe MainWindow
MainWindow::~MainWindow()
{
    delete ui; // Libération de la mémoire de l'interface utilisateur
}

// Bouton "Ajouter" cliqué
void MainWindow::on_Sp_Button_Ajouter_clicked()
{
    QString idSponsorString = ui->Sp_Line_ID->text().trimmed();
    QString nomSponsor = ui->Sp_Line_Nom->text().trimmed();
    QString emailSponsor = ui->Sp_Line_Email->text().trimmed();
    QString numtelSponsorString = ui->Sp_Line_Numtel->text().trimmed();
    QDate dateDebutSponsor = ui->dateEdit_Debut->date();
    QDate dateFinSponsor = ui->dateEdit_Fin->date();

    if (idSponsorString.isEmpty() || nomSponsor.isEmpty() || emailSponsor.isEmpty() || numtelSponsorString.isEmpty()) {
        ui->Sp_Label_InfoAffichage_2->setText("⚠️ Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int idSponsor = idSponsorString.toInt(&ok);
    if (!ok || idSponsor <= 0) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ L'ID doit être un entier positif.");
        return;
    }

    if (numtelSponsorString.length() != 8 || !numtelSponsorString.toLongLong(&ok) || !ok) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Le numéro doit contenir exactement 8 chiffres.");
        return;
    }

    if (dateFinSponsor < dateDebutSponsor) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ La date de fin doit être identique ou postérieure à la date de début.");
        return;
    }

    // Vérification ID existant
    QSqlQuery checkId;
    checkId.prepare("SELECT COUNT(*) FROM SPONSORS WHERE ID_SPONSOR = :id");
    checkId.bindValue(":id", idSponsor);
    checkId.exec();
    if (checkId.next() && checkId.value(0).toInt() > 0) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Cet ID existe déjà.");
        return;
    }

    // Vérification email existant
    QSqlQuery checkEmail;
    checkEmail.prepare("SELECT COUNT(*) FROM SPONSORS WHERE EMAIL_SPONSOR = :email");
    checkEmail.bindValue(":email", emailSponsor);
    checkEmail.exec();
    if (checkEmail.next() && checkEmail.value(0).toInt() > 0) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Cet email est déjà utilisé.");
        return;
    }

    int numtelSponsor = numtelSponsorString.toInt();

    Sponsor S(idSponsor, nomSponsor, emailSponsor, numtelSponsor, dateDebutSponsor, dateFinSponsor);
    if (S.ajouter()) {
        ui->Sp_Label_InfoAffichage_2->setText("✅ Ajout réussi (ID: " + QString::number(idSponsor) + ")");
        ui->Sp_TableView_Res->setModel(S.afficher());
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();
    } else {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Échec de l'ajout.");
    }
}

// Bouton "Modifier" cliqué
void MainWindow::on_Sp_Button_Modifier_clicked()
{
    QString idSponsorString = ui->Sp_Line_ID->text().trimmed();
    QString nomSponsor = ui->Sp_Line_Nom->text().trimmed();
    QString emailSponsor = ui->Sp_Line_Email->text().trimmed();
    QString numtelSponsorString = ui->Sp_Line_Numtel->text().trimmed();
    QDate dateDebutSponsor = ui->dateEdit_Debut->date();
    QDate dateFinSponsor = ui->dateEdit_Fin->date();

    if (idSponsorString.isEmpty() || nomSponsor.isEmpty() || emailSponsor.isEmpty() || numtelSponsorString.isEmpty()) {
        ui->Sp_Label_InfoAffichage_2->setText("⚠️ Tous les champs doivent être remplis.");
        return;
    }

    bool ok;
    int idSponsor = idSponsorString.toInt(&ok);
    if (!ok || idSponsor <= 0) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ ID invalide.");
        return;
    }

    if (numtelSponsorString.length() != 8 || !numtelSponsorString.toLongLong(&ok) || !ok) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Le numéro doit contenir exactement 8 chiffres.");
        return;
    }

    if (dateFinSponsor < dateDebutSponsor) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ La date de fin doit être identique ou postérieure à la date de début.");
        return;
    }

    // Vérifier que l'email n'appartient pas à un autre sponsor
    QSqlQuery checkEmail;
    checkEmail.prepare("SELECT ID_SPONSOR FROM SPONSORS WHERE EMAIL_SPONSOR = :email AND ID_SPONSOR != :id");
    checkEmail.bindValue(":email", emailSponsor);
    checkEmail.bindValue(":id", idSponsor);
    checkEmail.exec();
    if (checkEmail.next()) {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Cet email est déjà utilisé par un autre sponsor.");
        return;
    }

    int numtelSponsor = numtelSponsorString.toInt();

    Sponsor S(idSponsor, nomSponsor, emailSponsor, numtelSponsor, dateDebutSponsor, dateFinSponsor);
    if (S.modifier()) {
        ui->Sp_Label_InfoAffichage_2->setText("✅ Modification réussie (ID: " + QString::number(idSponsor) + ")");
        ui->Sp_TableView_Res->setModel(S.afficher());
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();
    } else {
        ui->Sp_Label_InfoAffichage_2->setText("❌ Échec de la modification.");
    }
}




void MainWindow::on_Sp_Button_Supprimer_clicked()
{
    // Récupération de l'ID sélectionné dans la combobox
    Sponsor S;
    S.setIdSponsor(ui->Sp_Combo_IDs->currentText().toInt());

    // Tentative de suppression du sponsor
    bool test = S.supprimer(S.getIdSponsor());
    if (test) {
        ui->Sp_Label_InfoAffichage_2->setText("Suppression Effectué"); // Changed label_info_gestion to Sp_Label_InfoAffichage
        ui->Sp_TableView_Res->setModel(S.afficher()); // Changed table_Clients to Sp_TableView_Res
        ui->Sp_Combo_IDs->setModel(S.afficher_id());
        clearFields();// Changed comboBox_IDs to Sp_Combo_IDs
    } else {
        ui->Sp_Label_InfoAffichage_2->setText("Suppression non effectué"); // Changed label_info_gestion to Sp_Label_InfoAffichage
    }
}

// Lorsque l'index de la combobox change
void MainWindow::on_Sp_Combo_IDs_currentIndexChanged(int index)
{
    // Récupération de l'ID sélectionné
    int idSponsor = ui->Sp_Combo_IDs->currentText().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM SPONSORS WHERE ID_SPONSOR = :ID_SPONSOR");
    query.bindValue(":ID_SPONSOR", idSponsor);

    // Remplissage des champs du formulaire avec les données du sponsor sélectionné
    if (query.exec()) {
        while (query.next()) {
            ui->Sp_Line_ID->setText(query.value(0).toString());
            ui->Sp_Line_Nom->setText(query.value(1).toString());
            ui->Sp_Line_Numtel->setText(query.value(2).toString());
            ui->Sp_Line_Email->setText(query.value(3).toString());
            ui->dateEdit_Debut->setDate(query.value(4).toDate());
            ui->dateEdit_Fin->setDate(query.value(5).toDate());
        }
    } else {
        ui->Sp_Label_InfoAffichage->setText("Échec de chargement");
    }
}

// Méthode pour effacer tous les champs du formulaire
void MainWindow::clearFields()
{
    // Nettoyage des champs QLineEdit
    ui->Sp_Line_ID->clear();
    ui->Sp_Line_Nom->clear();
    ui->Sp_Line_Email->clear();
    ui->Sp_Line_Numtel->clear();

    // Réinitialisation des champs QDateEdit à la date actuelle
    ui->dateEdit_Debut->setDate(QDate::currentDate());
    ui->dateEdit_Fin->setDate(QDate::currentDate());
}








void MainWindow::on_Sp_Button_ExportPDF_clicked()
{
    QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->Sp_TableView_Res->model()->rowCount();
        const int columnCount = ui->Sp_TableView_Res->model()->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               << QString("<title>%1</title>\n").arg("Export PDF")
               << "</head>\n"
               "<body bgcolor=#ffffff link=#5000A0>\n"
               "<center><h1>Liste des Recherches</h1><br><br>\n"
               "<table border=1 cellspacing=0 cellpadding=2>\n";

        // Table headers
        out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
        for (int column = 0; column < columnCount; ++column) {
            if (!ui->Sp_TableView_Res->isColumnHidden(column)) {
                out << QString("<th>%1</th>").arg(
                    ui->Sp_TableView_Res->model()->headerData(column, Qt::Horizontal).toString());
            }
        }
        out << "</tr></thead>\n";

        // Table data
        for (int row = 0; row < rowCount; ++row) {
            out << "<tr><td>" << row + 1 << "</td>";
            for (int column = 0; column < columnCount; ++column) {
                if (!ui->Sp_TableView_Res->isColumnHidden(column)) {
                    QString data = ui->Sp_TableView_Res->model()
                                       ->data(ui->Sp_TableView_Res->model()->index(row, column))
                                       .toString()
                                       .simplified();
                    out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
                }
            }
            out << "</tr>\n";
        }

        out << "</table></center>\n</body>\n</html>\n";

        QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", "", "*.pdf");
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setOutputFileName(fileName);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.print(&printer);
}



void MainWindow::on_Sp_Button_Tri_Numtel_clicked()
{
    ui->Sp_Label_InfoAffichage->setText("Tri par NUMÉRO DE TÉLÉPHONE effectué");
    ui->Sp_TableView_Res->setModel(S.Afficher_Tri_TEL());
}

void MainWindow::on_Sp_Button_Tri_Nom_clicked()
{
    ui->Sp_Label_InfoAffichage->setText("Tri par NOM effectué");
    ui->Sp_TableView_Res->setModel(S.Afficher_Tri_NOM());
}

void MainWindow::on_Sp_Button_Tri_Email_clicked()
{
    ui->Sp_Label_InfoAffichage->setText("Tri par EMAIL effectué");
    ui->Sp_TableView_Res->setModel(S.Afficher_Tri_EMAIL());
}



void MainWindow::on_Sp_Line_Recherche_textChanged(const QString &arg1)
{

        S.clearTable(ui->Sp_TableView_Res);
        S.Recherche(ui->Sp_TableView_Res, arg1);
        ui->Sp_TableView_Res->setColumnWidth(3, 250);
        ui->Sp_TableView_Res->setColumnWidth(2, 250);


}

