#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MallFlow");
    ui->Sp_TableView_Res->setModel(S.afficher());
    ui->Sp_TableView_Res->setColumnWidth(3, 250);
    ui->Sp_TableView_Res->setColumnWidth(2, 250);
    ui->Sp_Combo_IDs->setModel(S.afficher_id());
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    Sponsor S;
    S.setIdSponsor(ui->Sp_Combo_IDs->currentText().toInt()); // Changed comboBox_IDs to Sp_Combo_IDs
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

void MainWindow::on_Sp_Combo_IDs_currentIndexChanged(int index)
{
    int idSponsor = ui->Sp_Combo_IDs->currentText().toInt(); // Changed comboBox_IDs to Sp_Combo_IDs
    QString idSponsorString = QString::number(idSponsor);
    QSqlQuery query;
    query.prepare("SELECT * FROM SPONSORS WHERE ID_SPONSOR = :ID_SPONSOR"); // Changed CLIENT to SPONSORS and used parameter binding
    query.bindValue(":ID_SPONSOR", idSponsor);
    if (query.exec()) {
        while (query.next()) {
            ui->Sp_Line_ID->setText(query.value(0).toString()); // Changed line_ID to Sp_Line_ID
            ui->Sp_Line_Nom->setText(query.value(1).toString());
            ui->Sp_Line_Numtel->setText(query.value(2).toString());// Changed line_nom to Sp_Line_Nom
            ui->Sp_Line_Email->setText(query.value(3).toString()); // Changed line_email to Sp_Line_Email
         // Changed line_numtel to Sp_Line_Numtel
            ui->dateEdit_Debut->setDate(query.value(4).toDate()); // Added date handling
            ui->dateEdit_Fin->setDate(query.value(5).toDate()); // Added date handling
        }
    } else {
        ui->Sp_Label_InfoAffichage->setText("Echec de chargement"); // Changed label_info_gestion to Sp_Label_InfoAffichage
    }
}
void MainWindow::clearFields()
{
    // Clear all QLineEdit fields
    ui->Sp_Line_ID->clear();
    ui->Sp_Line_Nom->clear();
    ui->Sp_Line_Email->clear();
    ui->Sp_Line_Numtel->clear();

    // Reset QDateEdit fields to a default date (e.g., current date or a specific date)
    ui->dateEdit_Debut->setDate(QDate::currentDate()); // Or set to a specific default, e.g., QDate(2000, 1, 1)
    ui->dateEdit_Fin->setDate(QDate::currentDate());   // Or set to a specific default
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
           "<center><H1>Liste des Sponsors</H1><br><br>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numero</th>";
    for (int column = 0; column < columnCount; column++) {
        if (!ui->Sp_TableView_Res->isColumnHidden(column)) {
            out << QString("<th>%1</th>").arg(ui->Sp_TableView_Res->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; row++) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->Sp_TableView_Res->isColumnHidden(column)) {
                QString data = ui->Sp_TableView_Res->model()->data(ui->Sp_TableView_Res->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
            }
        }
        out << "</tr>\n";
    }

    out << "</table></center>\n</body>\n</html>\n";

    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size());
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
