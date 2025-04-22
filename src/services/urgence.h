#ifndef URGENCE_H
#define URGENCE_H

#include <QDialog>
#include <QSoundEffect>
#include <QDate>
#include <QTableView>
#include <QTabWidget>

class Urgence : public QDialog
{
    Q_OBJECT
public:
    explicit Urgence(const QString& message = "ALARME D'URGENCE\nÉVACUATION IMMÉDIATE", QWidget *parent = nullptr);
    ~Urgence();

    static void showDateAlert(const QString& title, const QString& message, QWidget *parent = nullptr);
    static void checkTodayDuplicates(QWidget *parent, QTableView *tableView, QTabWidget *tabWidget);

private:
    QSoundEffect *alarmSound;
    void initializeAlarmSound();
};

#endif // URGENCE_H
