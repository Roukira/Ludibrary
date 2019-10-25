#ifndef STATSMONITOR_H
#define STATSMONITOR_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QList>
#include <dbmanager.h>

class StatsMonitor : public QObject
{
    Q_OBJECT
public:
    explicit StatsMonitor(QObject *parent = nullptr);

    void updateGeneralStats();
    void updatePlatformStat(QString platform);
    void updateEmulatorStat(QString emulatorName);
    void updateExtSoftStat(QString extSoftName);

    void createPlatformStat(QString platform);
    void createEmulatorStat(QString emulatorName);
    void createExtSoftStat(QString extSoftName);

    bool platformStatsExist(QString platform);
    bool emulatorStatsExist(QString emulatorName);
    bool extSoftStatsExist(QString extSoftName);

    QString convertTimeToString(int timePlayed);
    QString convertGameCounterToString(int gameCounter);
    QString convertGameCounterToStringForPlatform(int gameCounter, QString platform);
    QString convertGameCounterToStringForEmulator(int gameCounter, QString emulatorName);
    QString convertPlatformCounterToString(int platformCounter);
    QString convertEmulatorCounterToString(int emulatorCounter);
    QString convertExtSoftCounterToString(int extSoftCounter);
    QString convertOpenedCounterToString(int openedCounter);


    void deletePlatformStat(QString platform);
    void deleteEmulatorStat(QString emulatorName);
    void deleteExtSoftStat(QString extSoftName);


signals:
    void updateGeneralQuery(QString games, QString platform, QString emulator, QString playtime, QString extsoft);
    void createPlatformQuery(QLabel* platformName, QLabel* platformGameCounter, QLabel* platformTimePlayed, QLabel* platformOpenedCounter);
    void createEmulatorQuery(QLabel* emulatorName, QLabel* emulatorGameCounter, QLabel* emulatorTimePlayed, QLabel* emulatorOpenedCounter);
    void createExtSoftQuery(QLabel* extSoftName, QLabel* extSoftOpenedCounter);

public slots:
    void updateStatistics();

private:
    int m_gameCounter;
    int m_platformCounter;
    int m_emulatorCounter;
    int m_totalPlayTime;
    int m_extSoftCounter;

    QList<QList<QLabel*> > m_platformList;
    QList<QList<QLabel*> > m_emulatorList;
    QList<QList<QLabel*> > m_extSoftList;

    QStringList m_platformListString;
    QStringList m_emulatorListString;
    QStringList m_extSoftListString;

    QTimer *m_timer;
    int m_timerInterval = 300000;


};

#endif // STATSMONITOR_H
