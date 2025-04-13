#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "services.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>
#include <QTextDocument>
#include <QDateTime>
#include <QTableView>
#include <QVBoxLayout>
#include <QDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QSqlError>
#include <QSqlDatabase>
#include "urgence.h"
#include <QSystemTrayIcon>
#include <QHBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QHeaderView>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation du graphique
    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background-color: rgba(255, 255, 255, 180); border: 1px solid #230a98; border-radius: 10px;");
    chartView->setFixedSize(500, 400);

    // Ajout du graphique au layout de tab_3
    QHBoxLayout *layout = new QHBoxLayout(ui->tab_3);
    layout->addWidget(ui->Sp_TableView_Res_3);
    layout->addWidget(chartView);
    ui->tab_3->setLayout(layout);

    // Connexions des boutons
    connect(ui->Sp_Button_Ajouter, &QPushButton::clicked, this, &MainWindow::onBtnAjouterClicked);
    connect(ui->Sp_Button_Modifier_3, &QPushButton::clicked, this, &MainWindow::onBtnModifierClicked);
    connect(ui->Sp_Button_Modifier_4, &QPushButton::clicked, this, &MainWindow::onBtnSupprimerClicked);
    connect(ui->Sp_Button_Tri_Nom, &QPushButton::clicked, this, &MainWindow::onBtnTriNomClicked);
    connect(ui->Sp_Button_Tri_Type, &QPushButton::clicked, this, &MainWindow::onBtnTriTypeClicked);
    connect(ui->Sp_Line_Recherche, &QLineEdit::textChanged, this, &MainWindow::onRechercheTextChanged);
    connect(ui->Sp_Button_auto, &QPushButton::clicked, this, &MainWindow::on_Sp_Button_auto_clicked);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::refreshStats);
    // Vérification des services avec la date d'aujourd'hui
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Services WHERE Date_Service = :today");
    query.bindValue(":today", QDate::currentDate().toString("yyyy-MM-dd"));
    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification des services du jour :" << query.lastError().text();
    } else {
        query.next();
        int count = query.value(0).toInt();
        if (count == 0) {
            // Aucun service aujourd'hui, ouvrir "Gestion des Services" (tab1)
            ui->tabWidget->setCurrentIndex(0);
        } else {
            // Des services existent, ouvrir "Liste des Services" (tab_2)
            ui->tabWidget->setCurrentIndex(1);
        }
    }
    // Style des tableaux
    styleTableView(ui->Sp_TableView_Res);
    styleTableView(ui->Sp_TableView_Res_3);
    onBtnAfficherClicked();
    // Initialisation des stats
    refreshStats();

    // Vérification des services d'aujourd'hui au démarrage
    QTimer::singleShot(1000, this, [this]() {
        Urgence::checkTodayDuplicates(this, ui->Sp_TableView_Res, ui->tabWidget);
    });
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);

    // Vérifier la connexion à la base de données
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Erreur : Base de données non connectée";
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.");
        return;
    }

    // Afficher la liste des services
    onBtnAfficherClicked();

    // Débogage
    QAbstractItemModel *model = ui->Sp_TableView_Res->model();
    qDebug() << "Nombre de lignes dans Sp_TableView_Res :" << (model ? model->rowCount() : 0);
}

void MainWindow::styleTableView(QTableView *tableView)
{
    tableView->setStyleSheet(
        "QTableView {"
        "   background-color: rgba(255, 255, 255, 200);"
        "   border: 2px solid #230a98;"
        "   border-radius: 8px;"
        "   alternate-background-color: #f0f5ff;"
        "   gridline-color: #c4c4c4;"
        "}"
        "QHeaderView::section {"
        "   background-color: #230a98;"
        "   color: white;"
        "   padding: 6px;"
        "   border: none;"
        "   font-weight: bold;"
        "   font-size: 10pt;"
        "}"
        "QTableView::item {"
        "   padding: 5px;"
        "}"
        "QTableView::item:selected {"
        "   background-color: #7359f0;"
        "   color: white;"
        "}"
        "QScrollBar::vertical {"
        "   border: 1px solid #c4c4c4;"
        "   background: #f0f0f0;"
        "   width: 12px;"
        "   margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #230a98;"
        "   min-height: 20px;"
        "   border-radius: 4px;"
        "}"
        );

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    tableView->verticalHeader()->setDefaultSectionSize(35);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSortingEnabled(true);
}

void MainWindow::updateChart()
{
    QSqlQuery query;
    query.prepare("SELECT Type_Service, COUNT(*) FROM Services GROUP BY Type_Service");

    if (!query.exec()) {
        qDebug() << "Erreur lors de la récupération des données pour le graphique:" << query.lastError().text();
        return;
    }

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        QPieSlice *slice = series->append(type, count);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(type).arg(count));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Services par Type");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setBackgroundBrush(QBrush(QColor(240, 240, 255)));
    chart->setTitleBrush(QBrush(QColor(35, 10, 152)));

    chartView->setChart(chart);
}

void MainWindow::loggerHistorique(const QString& action, int id_service, const QString& details)
{
    QSqlQuery query;
    query.prepare("INSERT INTO HistoriqueServices (action, id_service, details) "
                  "VALUES (:action, :id_service, :details)");
    query.bindValue(":action", action);
    query.bindValue(":id_service", id_service);
    query.bindValue(":details", details);

    if(!query.exec()) {
        qDebug() << "Erreur historique:" << query.lastError().text();
    }
}

void MainWindow::refreshStats()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT Type_Service AS \"Type\", COUNT(*) AS \"Nombre\", "
                  "ROUND(COUNT(*)*100.0/(SELECT COUNT(*) FROM Services), 1) AS \"Pourcentage %\" "
                  "FROM Services GROUP BY Type_Service");

    if (query.exec()) {
        model->setQuery(std::move(query));
        ui->Sp_TableView_Res_3->setModel(model);
        ui->Sp_TableView_Res_3->resizeColumnsToContents();
        updateChart();
    } else {
        qDebug() << "Erreur stats:" << query.lastError().text();
    }
}

void MainWindow::on_Sp_Button_Modifier_2_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(timestamp, 'DD/MM/YYYY HH24:MI') AS \"Date\", "
                    "action AS \"Action\", "
                    "id_service AS \"ID Service\", "
                    "details AS \"Détails\" "
                    "FROM HistoriqueServices ORDER BY timestamp DESC");

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Historique des Opérations");
    dialog->resize(1000, 600);

    dialog->setStyleSheet("QDialog {"
                          "background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f5f7fa, stop:1 #c3cfe2);"
                          "border: 2px solid #230a98;"
                          "border-radius: 10px;"
                          "}");

    QTableView *tableView = new QTableView(dialog);
    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    tableView->setStyleSheet("QTableView {"
                             "background-color: rgba(255, 255, 255, 200);"
                             "border: 1px solid #230a98;"
                             "border-radius: 5px;"
                             "alternate-background-color: #f0f5ff;"
                             "}"
                             "QHeaderView::section {"
                             "background-color: #230a98;"
                             "color: white;"
                             "padding: 5px;"
                             "border: none;"
                             "font-weight: bold;"
                             "}");

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setDefaultSectionSize(30);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(tableView);
    dialog->setLayout(layout);
    dialog->exec();
}

void MainWindow::on_Sp_Button_ExportPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    QString html = "<h1>Liste des Services</h1><table border='1'><tr>"
                   "<th>ID</th><th>Nom</th><th>Type</th><th>Date</th><th>État</th></tr>";

    QSqlQuery query("SELECT * FROM Services");
    while (query.next()) {
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td></tr>")
        .arg(query.value("ID_Service").toString())
            .arg(query.value("Nom_Service").toString())
            .arg(query.value("Type_Service").toString())
            .arg(query.value("Date_Service").toDate().toString("dd/MM/yyyy"))
            .arg(query.value("Etat").toString());
    }
    html += "</table>";
    doc.setHtml(html);
    doc.print(&printer);
    QMessageBox::information(this, "Succès", "PDF exporté avec succès");
}

void MainWindow::on_Sp_Button_auto_clicked()
{
    int id = ui->Sp_Line_ID_2->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT Nom_Service, Type_Service, Date_Service, Etat FROM Services WHERE ID_Service = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur DB: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        ui->Sp_Line_Nom_2->setText(query.value(0).toString());
        ui->comboBox->setCurrentText(query.value(1).toString());
        ui->dateEdit->setDate(query.value(2).toDate());
        ui->Sp_Line_Email_2->setText(query.value(3).toString());
    }
}

int MainWindow::getSelectedServiceID()
{
    QModelIndexList selectedIndexes = ui->Sp_TableView_Res->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un service");
        return -1;
    }
    return ui->Sp_TableView_Res->model()->data(selectedIndexes.at(0)).toInt();
}

void MainWindow::onBtnAjouterClicked()
{
    int id = ui->Sp_Line_ID_2->text().toInt();
    QString nom = ui->Sp_Line_Nom_2->text();
    QString type = ui->comboBox->currentText();
    QDate date = ui->dateEdit->date();
    QString etat = ui->Sp_Line_Email_2->text();

    if (nom.isEmpty() || type.isEmpty() || etat.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Remplissez tous les champs");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_Service FROM Services WHERE ID_Service = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        QMessageBox::warning(this, "Erreur", "ID existe déjà");
        return;
    }

    Services s(id, nom, type, date, etat);
    if (s.ajouter()) {
        QMessageBox::information(this, "Succès", "Service ajouté");
        loggerHistorique("Ajout", id, QString("Nom: %1, Type: %2").arg(nom).arg(type));
        onBtnAfficherClicked();
        refreshStats();
    }
}

void MainWindow::onBtnAfficherClicked()
{
    Services s;
    QSqlQueryModel *model = s.afficher();
    ui->Sp_TableView_Res->setModel(model);
    ui->Sp_TableView_Res->resizeColumnsToContents();
    qDebug() << "onBtnAfficherClicked: Modèle défini, lignes =" << (model ? model->rowCount() : 0);
}

void MainWindow::onBtnSupprimerClicked()
{
    int id = getSelectedServiceID();
    if (id == -1) return;

    Services s;
    if (s.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Service supprimé");
        loggerHistorique("Suppression", id, "Service supprimé");
        onBtnAfficherClicked();
        refreshStats();
    }
}

void MainWindow::onBtnModifierClicked()
{
    int id = ui->Sp_Line_ID_2->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_Service FROM Services WHERE ID_Service = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "ID n'existe pas");
        return;
    }

    QString nom = ui->Sp_Line_Nom_2->text();
    QString type = ui->comboBox->currentText();
    QDate date = ui->dateEdit->date();
    QString etat = ui->Sp_Line_Email_2->text();

    Services s(id, nom, type, date, etat);
    if (s.modifier(id)) {
        QMessageBox::information(this, "Succès", "Service modifié");
        loggerHistorique("Modification", id, QString("Nouveau nom: %1").arg(nom));
        onBtnAfficherClicked();
        refreshStats();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->show();
    trayIcon->showMessage("ALARME D'URGENCE",
                          "Évacuation immédiate requise !",
                          QSystemTrayIcon::Critical,
                          10000);

    Urgence *urgence = new Urgence("ALARME D'URGENCE\nÉVACUATION IMMÉDIATE", this);
    urgence->exec();
    delete urgence;
    delete trayIcon;
}

void MainWindow::onBtnTriNomClicked()
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.trierParNom());
}

void MainWindow::onBtnTriTypeClicked()
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.trierParType());
}

void MainWindow::onRechercheTextChanged(const QString &text)
{
    Services s;
    ui->Sp_TableView_Res->setModel(s.rechercherParType(text));
}

void MainWindow::on_Sp_Button_Fermer_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_Sp_Button_Fermer_2_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_Sp_Button_Fermer_4_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
