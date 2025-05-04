#include "urgence.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractItemModel>
#include <QItemSelectionModel>

Urgence::Urgence(const QString& message, QWidget *parent)
    : QDialog(parent), alarmSound(nullptr)
{
    // Configuration de la fenêtre
    bool isDateAlert = (message != "ALARME D'URGENCE\nÉVACUATION IMMÉDIATE");
    if (isDateAlert) {
        // Fenêtre réduite pour l'alerte de date
        setFixedSize(400, 300);
        setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
        // Centrer la fenêtre
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->availableGeometry();
            int x = (screenGeometry.width() - width()) / 2;
            int y = (screenGeometry.height() - height()) / 2;
            move(x, y);
        }
    } else {
        // Fenêtre plein écran pour l'alarme SOS
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QScreen *screen = QGuiApplication::primaryScreen();
        if (screen) {
            setGeometry(screen->geometry());
        } else {
            setGeometry(0, 0, 1920, 1080);
        }
        setWindowState(Qt::WindowFullScreen);
    }

    setStyleSheet("background-color: #ff0000;");

    // Message
    QLabel *label = new QLabel(message, this);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(isDateAlert ?
                             "font-size: 20px; font-weight: bold; color: white;" :
                             "font-size: 72px; font-weight: bold; color: white;");

    // Bouton
    QPushButton *quitButton = new QPushButton(isDateAlert ? "Fermer" : "DÉSACTIVER L'ALARME", this);
    quitButton->setStyleSheet("QPushButton {"
                              "background-color: white;"
                              "color: red;"
                              "font-size: 24px;"
                              "font-weight: bold;"
                              "padding: 15px;"
                              "border-radius: 10px;"
                              "border: 2px solid darkred;"
                              "margin: 20px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #ffcccc;"
                              "}");

    connect(quitButton, &QPushButton::clicked, this, &QDialog::accept);

    // Disposition
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(label);
    layout->addWidget(quitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    // Effet clignotant
    QTimer *blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, [label]() {
        label->setVisible(!label->isVisible());
    });
    blinkTimer->start(500);

    // Initialisation du son (seulement pour SOS)
    if (!isDateAlert) {
        initializeAlarmSound();
    }
}

void Urgence::initializeAlarmSound()
{
    // Vérifie si le fichier existe dans les ressources
    if (!QFile(":/imgs/alarm.wav").exists()) {
        qDebug() << "Fichier son alarm.wav introuvable dans les ressources";
        return;
    }

    alarmSound = new QSoundEffect(this);
    alarmSound->setSource(QUrl("qrc:/imgs/alarm.wav"));

    // Connecte le signal statusChanged pour détecter les erreurs
    connect(alarmSound, &QSoundEffect::statusChanged, [this]() {
        if (alarmSound->status() == QSoundEffect::Error) {
            qDebug() << "Erreur de lecture du son:" << alarmSound->source().toString();
        }
    });

    alarmSound->setLoopCount(QSoundEffect::Infinite);
    alarmSound->setVolume(0.9f);
    alarmSound->play();

    // Debug
    qDebug() << "Statut du son:" << alarmSound->status();
    qDebug() << "Source du son:" << alarmSound->source().toString();
    qDebug() << "Fichier existe:" << QFile(":/imgs/alarm.wav").exists();
    qDebug() << "Taille du fichier:" << QFile(":/imgs/alarm.wav").size();
}

Urgence::~Urgence()
{
    if (alarmSound) {
        alarmSound->stop();
        alarmSound->deleteLater();
    }
}

void Urgence::showDateAlert(const QString& title, const QString& message, QWidget *parent)
{
    Urgence dialog(message, parent);
    dialog.setWindowTitle(title);
    dialog.exec();
}

void Urgence::checkTodayDuplicates(QWidget *parent, QTableView *tableView, QTabWidget *tabWidget)
{
    QDate today = QDate::currentDate();
    QSqlQuery query;
    query.prepare("SELECT ID_Service, Nom_Service FROM Services WHERE Date_Service = :date");
    query.bindValue(":date", today.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification des dates:" << query.lastError().text();
        return;
    }

    QStringList todayServices;
    while (query.next()) {
        todayServices << QString("ID: %1 - %2").arg(query.value(0).toString()).arg(query.value(1).toString());
    }

    if (!todayServices.isEmpty()) {
        QString message = QString("%1 A faire pour aujourd'hui (%2):\n\n%3")
                              .arg(todayServices.count())
                              .arg(today.toString("dd/MM/yyyy"))
                              .arg(todayServices.join("\n"));

        // Afficher l'alerte
        showDateAlert("Services d'aujourd'hui", message, parent);

        // Basculer vers l'onglet "Liste des Services"
        tabWidget->setCurrentIndex(1);

        // Mettre en surbrillance les services d'aujourd'hui
        QAbstractItemModel *model = tableView->model();
        QItemSelectionModel *selectionModel = tableView->selectionModel();
        selectionModel->clearSelection();

        for (int row = 0; row < model->rowCount(); ++row) {
            QModelIndex dateIndex = model->index(row, 3); // Colonne Date
            if (model->data(dateIndex).toDate() == today) {
                tableView->selectRow(row);
                tableView->scrollTo(dateIndex, QAbstractItemView::PositionAtCenter);
            }
        }
    }
}
