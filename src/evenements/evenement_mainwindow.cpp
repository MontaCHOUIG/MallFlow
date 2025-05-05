#include "evenement_mainwindow.h"
#include "ui_evenement_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QIntValidator>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QDesktopServices>
#include <QUrl>
#include <QImage>
#include <QSqlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBuffer>
#include <QPixmap>
#include <QLabel>
#include <QQuickItem>
#include <QRegularExpression>
#include <QtCharts>

EvenementMainWindow::EvenementMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EvenementMainWindow)
{
    ui->setupUi(this);

    // Connexions des boutons de navigation
    connect(ui->employes, &QPushButton::clicked, this, &EvenementMainWindow::on_btnEmployees_clicked);
    connect(ui->sponsors, &QPushButton::clicked, this, &EvenementMainWindow::on_btnSponsors_clicked);
    connect(ui->services, &QPushButton::clicked, this, &EvenementMainWindow::on_btnServices_clicked);
    connect(ui->fournisseurs, &QPushButton::clicked, this, &EvenementMainWindow::on_btnSuppliers_clicked);
    connect(ui->magasins, &QPushButton::clicked, this, &EvenementMainWindow::on_btnStores_clicked);


    // connect logout signal
   // connect(ui->button_logout, &QPushButton::clicked, this, &EvenementMainWindow::on_button_logout_clicked);

    updateAuthUserLabel();

    // Initialisation de la carte
    mapWidget = new QQuickWidget(ui->tab_5);
    mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    mapWidget->setSource(QUrl("qrc:/map.qml"));
    ui->verticalLayout_tab5->addWidget(mapWidget);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &EvenementMainWindow::onNetworkReplyFinished);

    // Initialisation des graphiques
    chartViewParticipants = new QChartView(ui->tab_3);
    chartViewLieux = new QChartView(ui->tab_3);

    // Ajout au layout de tab3
    ui->verticalLayout_stats->addWidget(chartViewParticipants);
    ui->verticalLayout_stats->addWidget(chartViewLieux);

    updateStats(); // Mettre à jour les stats au lancement

    // Configuration des validateurs
    ui->ID_EVENEMENT->setValidator(new QIntValidator(1, 1000000, this));
    ui->ID_EMPLOYE->setValidator(new QIntValidator(1, 1000000, this));
    ui->ID_SPONSOR->setValidator(new QIntValidator(1, 1000000, this));

    afficherEvenements(); // Initialiser tableau et combo box

    updateQRCodeComboBox(); // Appelé à la fin
}

EvenementMainWindow::~EvenementMainWindow() {
    delete ui;
}

void EvenementMainWindow::on_button_logout_clicked() {
    emit logoutRequested();
}

bool EvenementMainWindow::recordExists(const QString &table, const QString &col, int id) {
    QSqlQuery q;
    q.prepare(QString("SELECT 1 FROM %1 WHERE %2 = :id").arg(table, col));
    q.bindValue(":id", id);
    if (!q.exec()) return false;
    return q.next();
}


void EvenementMainWindow::updateAuthUserLabel() {
    Employe emp;
    ui->label_authUser->setText("Logged in as: " + emp.getAuthenticatedUser());
}
void EvenementMainWindow::on_Sp_Button_Ajouter_clicked() {
    int id = ui->ID_EVENEMENT->text().toInt();
    QString titre = ui->TITRE->text().trimmed();
    QDate dateDebut = ui->DATE_DEBUT->date();
    QDate dateFin = ui->DATE_FIN->date();
    QString lieu = ui->LIEU->text().trimmed();
    QString typeParticipants = ui->TYPE_DE_PARTICIPANTS->text().trimmed();
    int idEmploye = ui->ID_EMPLOYE->text().toInt();
    int idSponsor = ui->ID_SPONSOR->text().toInt();

    if (id <= 0 || titre.isEmpty() || !dateDebut.isValid() || !dateFin.isValid() || dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "Champs invalides.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_EVENEMENT FROM SHOPDEVS.EVENEMENTS WHERE ID_EVENEMENT = :id");
    checkQuery.bindValue(":id", id);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "L'ID existe déjà !");
        return;
    }

    if (idEmploye > 0 && !recordExists("SHOPDEVS.EMPLOYES", "ID_EMPLOYE", idEmploye)) {
        QMessageBox::warning(this, "Erreur", "Employé inexistant.");
        return;
    }
    if (idSponsor > 0 && !recordExists("SHOPDEVS.SPONSORS", "ID_SPONSOR", idSponsor)) {
        QMessageBox::warning(this, "Erreur", "Sponsor inexistant.");
        return;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO SHOPDEVS.EVENEMENTS "
        "(ID_EVENEMENT, TITRE, DATE_DEBUT, DATE_FIN, LIEU, TYPE_DE_PARTICIPANTS, ID_EMPLOYE, ID_SPONSOR) "
        "VALUES (:id, :titre, TO_DATE(:debut, 'YYYY-MM-DD'), TO_DATE(:fin, 'YYYY-MM-DD'), :lieu, :type, :emp, :spon)"
        );
    insertQuery.bindValue(":id", id);
    insertQuery.bindValue(":titre", titre);
    insertQuery.bindValue(":debut", dateDebut.toString("yyyy-MM-dd"));
    insertQuery.bindValue(":fin", dateFin.toString("yyyy-MM-dd"));
    insertQuery.bindValue(":lieu", lieu);
    insertQuery.bindValue(":type", typeParticipants);
    insertQuery.bindValue(":emp", idEmploye);
    insertQuery.bindValue(":spon", idSponsor);
    if (insertQuery.exec()) {
        QMessageBox::information(this, "Succès", "Événement ajouté !");
        ui->ID_EVENEMENT->clear(); ui->TITRE->clear(); ui->DATE_DEBUT->setDate(QDate::currentDate());
        ui->DATE_FIN->setDate(QDate::currentDate()); ui->LIEU->clear(); ui->TYPE_DE_PARTICIPANTS->clear();
        ui->ID_EMPLOYE->clear(); ui->ID_SPONSOR->clear();

        afficherEvenements();
        updateStats();
        updateQRCodeComboBox();
    } else {
        QMessageBox::critical(this, "Erreur", insertQuery.lastError().text());
    }
}

void EvenementMainWindow::on_Sp_Button_Modifier_clicked() {
    int id = ui->Sp_combo_ID->currentText().toInt();
    QString titre = ui->TITRE->text();
    QDate dateDebut = ui->DATE_DEBUT->date();
    QDate dateFin = ui->DATE_FIN->date();
    QString lieu = ui->LIEU->text();
    QString typeParticipants = ui->TYPE_DE_PARTICIPANTS->text();
    int idEmploye = ui->ID_EMPLOYE->text().toInt();
    int idSponsor = ui->ID_SPONSOR->text().toInt();

    QSqlQuery query;
    query.prepare("UPDATE SHOPDEVS.EVENEMENTS SET TITRE=:titre, DATE_DEBUT=TO_DATE(:debut, 'YYYY-MM-DD'), DATE_FIN=TO_DATE(:fin, 'YYYY-MM-DD'), LIEU=:lieu, TYPE_DE_PARTICIPANTS=:type, ID_EMPLOYE=:emp, ID_SPONSOR=:spon WHERE ID_EVENEMENT=:id");
    query.bindValue(":titre", titre);
    query.bindValue(":debut", dateDebut.toString("yyyy-MM-dd"));
    query.bindValue(":fin", dateFin.toString("yyyy-MM-dd"));
    query.bindValue(":lieu", lieu);
    query.bindValue(":type", typeParticipants);
    query.bindValue(":emp", idEmploye);
    query.bindValue(":spon", idSponsor);
    query.bindValue(":id", id);

    if (query.exec()) {
        afficherEvenements();
        updateStats();
        updateQRCodeComboBox();
        QMessageBox::information(this, "Succès", "Modification réussie.");
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}

void EvenementMainWindow::on_Sp_Button_Supprimer_clicked() {
    int id = ui->Sp_combo_ID->currentText().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM SHOPDEVS.EVENEMENTS WHERE ID_EVENEMENT = :id");
    query.bindValue(":id", id);
    if (query.exec()) {
        afficherEvenements();
        updateStats();
        updateQRCodeComboBox();
        QMessageBox::information(this, "Succès", "Suppression réussie.");
    } else {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}

void EvenementMainWindow::on_Sp_combo_ID_currentIndexChanged(int index)
{
    QString idStr = ui->Sp_combo_ID->itemText(index);
    if (idStr.isEmpty()) return;

    QSqlQuery query;
    query.prepare("SELECT * FROM SHOPDEVS.EVENEMENTS WHERE ID_EVENEMENT = :id");
    query.bindValue(":id", idStr.toInt());
    if (query.exec() && query.next()) {
        ui->ID_EVENEMENT->setText(query.value("ID_EVENEMENT").toString());
        ui->TITRE->setText(query.value("TITRE").toString());
        ui->DATE_DEBUT->setDate(query.value("DATE_DEBUT").toDate());
        ui->DATE_FIN->setDate(query.value("DATE_FIN").toDate());
        ui->LIEU->setText(query.value("LIEU").toString());
        ui->TYPE_DE_PARTICIPANTS->setText(query.value("TYPE_DE_PARTICIPANTS").toString());
        ui->ID_EMPLOYE->setText(query.value("ID_EMPLOYE").toString());
        ui->ID_SPONSOR->setText(query.value("ID_SPONSOR").toString());
        QString lieu = query.value("LIEU").toString();
        if (!lieu.isEmpty()) {
            geocodeAddress(lieu);
        }
    }
}

void EvenementMainWindow::afficherEvenements(const QString &orderBy) {
    QString queryStr = "SELECT * FROM SHOPDEVS.EVENEMENTS";
    if (!orderBy.isEmpty()) queryStr += " ORDER BY " + orderBy;

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"ID", "Titre", "Date Début", "Date Fin", "Lieu", "Participants", "ID Employé", "ID Sponsor"});

    QSqlQuery query(queryStr);
    while (query.next()) {
        QList<QStandardItem*> row;
        for (int i = 0; i < 8; ++i) {
            row << new QStandardItem(query.value(i).toString());
        }
        model->appendRow(row);
    }

    ui->Sp_TableView_Res->setModel(model);
    ui->Sp_combo_ID->clear();
    QSqlQuery idQuery("SELECT ID_EVENEMENT FROM SHOPDEVS.EVENEMENTS");
    while (idQuery.next()) ui->Sp_combo_ID->addItem(idQuery.value(0).toString());
}

void EvenementMainWindow::on_Sp_Button_Recherche_clicked() {
    QString searchText = ui->Sp_Line_Recherche->text().trimmed();
    QSqlQuery query;
    query.prepare("SELECT * FROM SHOPDEVS.EVENEMENTS WHERE TITRE LIKE :search OR LIEU LIKE :search");
    query.bindValue(":search", "%" + searchText + "%");
    query.exec();

    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"ID", "Titre", "Date Début", "Date Fin", "Lieu", "Participants", "ID Employé", "ID Sponsor"});

    while (query.next()) {
        QList<QStandardItem*> row;
        for (int i = 0; i < 8; ++i) row << new QStandardItem(query.value(i).toString());
        model->appendRow(row);
    }

    ui->Sp_TableView_Res->setModel(model);
}

void EvenementMainWindow::on_Sp_Button_Tri_ID_clicked() {
    afficherEvenements("ID_EVENEMENT ASC");
}

void EvenementMainWindow::on_Sp_Button_Tri_Titre_clicked() {
    afficherEvenements("TITRE ASC");
}

void EvenementMainWindow::on_Sp_Button_Tri_Lieu_clicked() {
    afficherEvenements("LIEU ASC");
}

void EvenementMainWindow::on_Sp_Button_PDF_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;
    genererPDF(fileName);
}

void EvenementMainWindow::genererPDF(const QString &fileName) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 20, 15, 15));

    QString html = "<div style='display: flex; margin-bottom: 25px; padding-bottom: 15px; border-bottom: 2px solid #2E86C1;'>"
                   "<img src=':/imgs/logo.png' width='120' style='margin-right: 30px;'/>"
                   "<div style='flex-grow: 1; text-align: center;'>"
                   "<h1 style='color: #2E86C1; margin: 0; font-family: Arial;'>Événements MallFlow</h1>"
                   "<p style='color: #6C757D; font-size: 12px; margin-top: 3px;'>"
                   "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>"
                   "</div></div>";

    html += "<style>"
            "table { margin: 0 auto; width: 98%; border: 1px solid #ddd; }"
            "th, td { padding: 8px; text-align: center; }"
            "td:nth-child(2) { text-align: left; }"
            "th { background: #f8f9fa; color: #2E86C1; }"
            "</style>";

    html += "<table>"
            "<tr>"
            "<th>ID</th><th>Titre</th><th>Date Début</th><th>Date Fin</th>"
            "<th>Lieu</th><th>Participants</th><th>Employé</th><th>Sponsor</th>"
            "</tr>";

    QSqlQuery query("SELECT * FROM SHOPDEVS.EVENEMENTS");
    while (query.next()) {
        html += QString("<tr>"
                        "<td>%1</td><td>%2</td><td>%3</td><td>%4</td>"
                        "<td>%5</td><td>%6</td><td>%7</td><td>%8</td></tr>")
                    .arg(query.value(0).toString(),
                         query.value(1).toString(),
                         query.value(2).toDate().toString("dd/MM/yyyy"),
                         query.value(3).toDate().toString("dd/MM/yyyy"),
                         query.value(4).toString(),
                         query.value(5).toString(),
                         query.value(6).toString(),
                         query.value(7).toString());
    }

    html += "</table>";

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(printer.pageRect(QPrinter::Point).size());
    doc.print(&printer);

    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void EvenementMainWindow::updateStats() {
    QSqlQuery queryParticipants("SELECT TYPE_DE_PARTICIPANTS, COUNT(*) FROM SHOPDEVS.EVENEMENTS GROUP BY TYPE_DE_PARTICIPANTS");
    QPieSeries *seriesParticipants = new QPieSeries();

    while (queryParticipants.next()) {
        seriesParticipants->append(
            queryParticipants.value(0).toString(),
            queryParticipants.value(1).toInt()
            );
    }

    QChart *chartParticipants = new QChart();
    chartParticipants->addSeries(seriesParticipants);
    chartParticipants->setTitle("Répartition par type de participants");
    chartViewParticipants->setChart(chartParticipants);

    QSqlQuery queryLieux("SELECT LIEU, COUNT(*) FROM SHOPDEVS.EVENEMENTS GROUP BY LIEU");
    QBarSeries *seriesLieux = new QBarSeries();

    while (queryLieux.next()) {
        QBarSet *set = new QBarSet(queryLieux.value(0).toString());
        *set << queryLieux.value(1).toInt();
        seriesLieux->append(set);
    }

    QChart *chartLieux = new QChart();
    chartLieux->addSeries(seriesLieux);
    chartLieux->setTitle("Nombre d'événements par lieu");

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    chartLieux->addAxis(axisY, Qt::AlignLeft);
    seriesLieux->attachAxis(axisY);

    chartViewLieux->setChart(chartLieux);
}

void EvenementMainWindow::geocodeAddress(const QString &address)
{
    QUrl url(QString("https://nominatim.openstreetmap.org/search?format=json&q=%1").arg(QUrl::toPercentEncoding(address)));
    currentGeocodeUrl = url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "MallFlow/1.0");

    networkManager->get(request);
}

void EvenementMainWindow::generateQRCodeViaAPI(const QString &data)
{
    QString encodedData = QUrl::toPercentEncoding(data);
    QUrl url("https://api.qrserver.com/v1/create-qr-code/?size=200x200&data=" + encodedData);
    currentQRCodeUrl = url;

    QNetworkRequest request(url);
    networkManager->get(request);
}

void EvenementMainWindow::onNetworkReplyFinished(QNetworkReply *reply)
{
    QUrl requestedUrl = reply->request().url();

    if (requestedUrl == currentQRCodeUrl) {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        ui->label_3->setPixmap(pixmap);
    }
    else if (requestedUrl == currentGeocodeUrl) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonArray array = doc.array();

        QQuickItem *item = mapWidget->rootObject();

        if (!array.isEmpty() && item) {
            QJsonObject obj = array.first().toObject();
            double lat = obj["lat"].toString().toDouble();
            double lon = obj["lon"].toString().toDouble();

            QObject *root = static_cast<QObject*>(item);

            QMetaObject::invokeMethod(root, "clearMarkers");
            QMetaObject::invokeMethod(root, "addMarker", Q_ARG(qreal, lat), Q_ARG(qreal, lon));
            QMetaObject::invokeMethod(root, "centerOn", Q_ARG(qreal, lat), Q_ARG(qreal, lon));
        }
    }

    reply->deleteLater();
}

void EvenementMainWindow::on_tab4_combo_Event_currentIndexChanged(int index)
{
    QString idStr = ui->tab4_combo_Event->itemText(index);
    if (idStr.isEmpty()) return;

    QSqlQuery query;
    query.prepare("SELECT * FROM SHOPDEVS.EVENEMENTS WHERE ID_EVENEMENT = :id");
    query.bindValue(":id", idStr.toInt());

    if (query.exec() && query.next()) {
        QString info;
        info += "ID: " + query.value("ID_EVENEMENT").toString() + "\n";
        info += "Titre: " + query.value("TITRE").toString() + "\n";
        info += "Date début: " + query.value("DATE_DEBUT").toDate().toString("dd/MM/yyyy") + "\n";
        info += "Date fin: " + query.value("DATE_FIN").toDate().toString("dd/MM/yyyy") + "\n";
        info += "Lieu: " + query.value("LIEU").toString() + "\n";
        info += "Participants: " + query.value("TYPE_DE_PARTICIPANTS").toString() + "\n";
        info += "Employé: " + query.value("ID_EMPLOYE").toString() + "\n";
        info += "Sponsor: " + query.value("ID_SPONSOR").toString();

        info = info.normalized(QString::NormalizationForm_D).remove(QRegularExpression("[^a-zA-Z0-9\\s:/.-]"));

        generateQRCodeViaAPI(info);
    } else {
        ui->label_3->clear();
        qDebug() << "Échec récupération données événement.";
    }
}

void EvenementMainWindow::updateQRCodeComboBox()
{
    ui->tab4_combo_Event->clear();
    QSqlQuery query("SELECT ID_EVENEMENT FROM SHOPDEVS.EVENEMENTS");
    while (query.next()) {
        ui->tab4_combo_Event->addItem(query.value(0).toString());
    }
}

void EvenementMainWindow::on_btnEmployees_clicked() {
    Employe emp;
    if (emp.hasWindowAccess("employees")) {
        emit navigateTo(0); // Navigate to employees window
    } else {
        QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les permissions nécessaires pour accéder à cette fenêtre.");
    }
}

void EvenementMainWindow::on_btnSponsors_clicked() {
    Employe emp;
    if (emp.hasWindowAccess("sponsors")) {
        emit navigateTo(1); // Navigate to sponsors window
    } else {
        QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les permissions nécessaires pour accéder à cette fenêtre.");
    }
}

void EvenementMainWindow::on_btnServices_clicked() {
    Employe emp;
    if (emp.hasWindowAccess("services")) {
        emit navigateTo(2); // Navigate to services window
    } else {
        QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les permissions nécessaires pour accéder à cette fenêtre.");
    }
}

void EvenementMainWindow::on_btnSuppliers_clicked() {
    Employe emp;
    if (emp.hasWindowAccess("fournisseurs")) {
        emit navigateTo(4); // Navigate to suppliers window
    } else {
        QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les permissions nécessaires pour accéder à cette fenêtre.");
    }
}

void EvenementMainWindow::on_btnStores_clicked() {
    Employe emp;
    if (emp.hasWindowAccess("magasins")) {
        emit navigateTo(5); // Navigate to stores window
    } else {
        QMessageBox::warning(this, "Accès refusé", "Vous n'avez pas les permissions nécessaires pour accéder à cette fenêtre.");
    }
}
