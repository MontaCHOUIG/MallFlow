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
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtGlobal>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>


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
    checkDateFinAndSendMessage();
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


void MainWindow::tech_choix_pie() {
    QChartView *chartView;
    QSqlQuery query;
    qreal tot = 0, lessThan6Months = 0, between6MonthsAnd1Year = 0, moreThan1Year = 0;

    // Step 1: Get the total count of sponsors
    query.prepare("SELECT COUNT(*) FROM SPONSORS");
    if (query.exec() && query.next()) {
        tot = query.value(0).toDouble();
    } else {
        qDebug() << "Error getting total count:" << query.lastError().text();
        ui->Sp_Label_Stats->setText("❌ Erreur lors du calcul du total des sponsors.");
        return;
    }

    // Step 2: Calculate durations and classify them using MONTHS_BETWEEN for Oracle
    query.prepare(
        "WITH DurationData AS ("
        "    SELECT "
        "        CASE "
        "            WHEN MONTHS_BETWEEN(DATE_FIN, DATE_DEBUT) < 6 THEN 'Less than 6 months' "
        "            WHEN MONTHS_BETWEEN(DATE_FIN, DATE_DEBUT) BETWEEN 6 AND 12 THEN 'Between 6 months and 1 year' "
        "            ELSE 'More than 1 year' "
        "        END AS duration_category "
        "    FROM SPONSORS "
        "    WHERE DATE_DEBUT IS NOT NULL AND DATE_FIN IS NOT NULL"
        ")"
        "SELECT duration_category, COUNT(*) "
        "FROM DurationData "
        "GROUP BY duration_category"
    );

    if (query.exec()) {
        while (query.next()) {
            QString category = query.value(0).toString();
            int count = query.value(1).toInt();
            if (category == "Less than 6 months") {
                lessThan6Months = count;
            } else if (category == "Between 6 months and 1 year") {
                between6MonthsAnd1Year = count;
            } else if (category == "More than 1 year") {
                moreThan1Year = count;
            }
        }
    } else {
        qDebug() << "Error calculating durations:" << query.lastError().text();
        ui->Sp_Label_Stats->setText("❌ Erreur lors du calcul des durées: " + query.lastError().text());
        return;
    }

    // Step 3: Calculate percentages
    qreal c1 = (tot > 0) ? (lessThan6Months / tot) : 0;
    qreal c2 = (tot > 0) ? (between6MonthsAnd1Year / tot) : 0;
    qreal c3 = (tot > 0) ? (moreThan1Year / tot) : 0;

    // Step 4: Create the pie series
    QPieSeries *series = new QPieSeries();
    series->append("Moins de 6 mois", c1);
    series->append("Entre 6 mois et 1 an", c2);
    series->append("Plus d'1 an", c3);

    // Set labels for each slice
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }

    // Step 5: Create the chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Sponsors par Durée");
    chart->legend()->show();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeQt);

    // Step 6: Create the chart view
    chartView = new QChartView(chart, ui->Sp_Label_Stats);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(570, 570);

    // Add the chart view to the layout
    QVBoxLayout *layout = new QVBoxLayout(ui->Sp_Label_Stats);
    layout->addWidget(chartView);
    ui->Sp_Label_Stats->setLayout(layout);

    chartView->show();
}





void MainWindow::on_pushButton_Stat_clicked()
{
    tech_choix_pie();
    show();
}

void MainWindow::highlightDates() {
    QSqlQuery query;
    // Query the SPONSOR table for DATE_DEBUT and DATE_FIN
    if (query.exec("SELECT DATE_DEBUT, DATE_FIN FROM SPONSORS")) {
        while (query.next()) {
            // Retrieve the dates as strings and convert to QDateTime
            QString dateDebutString = query.value(0).toString();
            QString dateFinString = query.value(1).toString();
            QDateTime dateDebut = QDateTime::fromString(dateDebutString, Qt::ISODate);
            QDateTime dateFin = QDateTime::fromString(dateFinString, Qt::ISODate);
            QDate dateDebutDate = dateDebut.date();
            QDate dateFinDate = dateFin.date();

            // Highlight DATE_DEBUT in green
            if (dateDebutDate.isValid()) {
                QTextCharFormat format;
                format.setBackground(QBrush(QColor("#43a341"))); // Green background
                format.setForeground(QBrush(Qt::black));
                format.setFontWeight(QFont::Bold);
                ui->calendarWidget_sp->setDateTextFormat(dateDebutDate, format);
            }

            // Highlight DATE_FIN in red
            if (dateFinDate.isValid()) {
                QTextCharFormat format;
                format.setBackground(QBrush(QColor("#a35941"))); // Red background
                format.setForeground(QBrush(Qt::black));
                format.setFontWeight(QFont::Bold);
                ui->calendarWidget_sp->setDateTextFormat(dateFinDate, format);
            }
        }
    } else {
        // Handle query failure (optional)
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}


void MainWindow::on_calendarWidget_sp_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget_sp->selectedDate();
    QSqlQuery query;
    bool dateFound = false;

    // Query the SPONSOR table for all columns
    if (query.exec("SELECT * FROM SPONSORS")) {
        while (query.next()) {
            // Map the columns based on your table structure
            QString idSponsor = query.value(0).toString(); // ID_SPONSOR
            QString nomSponsor = query.value(1).toString(); // NOM_SPONSOR
            QString telSponsor = query.value(2).toString(); // TEL_SPONSOR
            QString emailSponsor = query.value(3).toString(); // EMAIL_SPONSOR
            QDate dateDebut = query.value(4).toDate(); // DATE_DEBUT
            QDate dateFin = query.value(5).toDate(); // DATE_FIN

            // Check if the selected date matches DATE_DEBUT or DATE_FIN
            if (selectedDate == dateDebut || selectedDate == dateFin) {
                ui->Calendrier_Info->setText(
                    "Sponsor Details:<br>"
                    "<b>ID Sponsor:</b> " + idSponsor + "<br>" +
                    "<b>Name:</b> " + nomSponsor + "<br>" +
                    "<b>Phone:</b> " + telSponsor + "<br>" +
                    "<b>Email:</b> " + emailSponsor + "<br>" +
                    "<b>Start Date:</b> " + dateDebut.toString("dd/MM/yyyy") + "<br>" +
                    "<b>End Date:</b> " + dateFin.toString("dd/MM/yyyy") + "<br>"
                );
                dateFound = true;
                break;
            }
        }
        if (!dateFound) {
            ui->Calendrier_Info->setText("No sponsor found for this date.");
        }
    } else {
        // Handle query failure (optional)
        ui->Calendrier_Info->setText("Error retrieving sponsor data.");
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}
void MainWindow::checkDateFinAndSendMessage() {
    // Get the current date
    QDate currentDate = QDate::currentDate();

    // Query the SPONSOR table to get NOM_SPONSOR, TEL_SPONSOR, and DATE_FIN
    QSqlQuery query;
    if (query.exec("SELECT NOM_SPONSOR, TEL_SPONSOR, DATE_FIN FROM SPONSORS")) {
        while (query.next()) {
            // Retrieve the sponsor's details
            QString nomSponsor = query.value(0).toString(); // NOM_SPONSOR
            QString telSponsor = query.value(1).toString(); // TEL_SPONSOR
            QDate dateFin = query.value(2).toDate(); // DATE_FIN

            // Check if the current date matches DATE_FIN
            if (currentDate == dateFin) {
                // Construct the longer French message for the sponsor
                QString message = "Bonjour " + nomSponsor + ", nous vous informons que votre contrat de sponsoring avec nous a pris fin aujourd'hui. "
                                 "n’hésitez pas à nous contacter. Cordialement, MallFlow.";

                // Send the post request using the sponsor's phone number
                if (!telSponsor.isEmpty()) { // Ensure TEL_SPONSOR is not NULL or empty
                    S.postrequest(message, telSponsor);

                    // Show a popup to the user confirming the message was sent
                    QMessageBox::information(
                        this,
                        "Notification Envoyée",
                        "Un message a été envoyé à " + nomSponsor + " pour l'informer que son contrat a pris fin.",
                        QMessageBox::Ok
                    );
                } else {
                    qDebug() << "No phone number available for sponsor:" << nomSponsor;

                    // Optional: Show a popup if the message couldn't be sent due to missing phone number
                    QMessageBox::warning(
                        this,
                        "Échec de l'Envoi",
                        "Échec de l'envoi du message à " + nomSponsor + " : numéro de téléphone manquant.",
                        QMessageBox::Ok
                    );
                }
            }
        }
    } else {
        // Handle query failure
        qDebug() << "Error executing query:" << query.lastError().text();

        // Show a popup for query failure
        QMessageBox::critical(
            this,
            "Erreur",
            "Erreur lors de la récupération des données des sponsors : " + query.lastError().text(),
            QMessageBox::Ok
        );
    }

    // Refresh the calendar highlights after checking
    highlightDates();
}
