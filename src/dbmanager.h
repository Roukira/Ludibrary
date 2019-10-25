#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSql>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QList>
#include <QDateTime>


class DbManager
{

public:
    DbManager(const QString& path);

    QString PlatformToSupportedEmulator(QString platform);
    QStringList SupportedEmulatorToPlatforms(QString emulatorName);
    void initDb(const QString& db_name);
    void close();
    void setDatabaseName(const QString& path);
    bool checkAddGamePossible(int id);

    bool addGameEntry(int id, QString gameTitle, QString platform, int steamid); //replace with Game* game
    bool deleteGameEntry(int id);
    int getGameId(QString gameTitle, QString platform);
    int getGameSteamId(QString gameTitle, QString platform);
    QList<int> getAllGames();
    QString getGameTitle(int id);
    QString getGamePlatform(int id);
    QString getGamePath(int id);
    QByteArray getGameImage(int id);
    QString getGameInfo(int id);
    QString getGameEmulator(int id);
    QString getGameReleaseDate(int id);
    int getGameTimePlayed(int id);
    QString getGameLastPlayed(int id); //
    int getGameOpenedCounter(int id);
    QString getGameState(int id); //

    QDateTime stringToDate(QString dateString);

    //bool setGameTitle(int id, QString gameTitle);
    //bool setGamePlatform(int id, QString platform);
    bool setGamePath(int id, QString gamePath);
    bool setGameImage(int id, QByteArray imageData);
    bool setGameInfo(int id, QString gameInfo);
    //bool setGameEmulator(int id, QString emulatorName);
    //bool setGameReleaseDate(int id, QString releaseDate);
    bool setGameTimePlayed(int id, int timePlayed);
    bool setGameOpenedCounter(int id, int openedCounter);
    bool updateGameLastPlayed(int id); //
    bool setGameState(int id, QString state);

    bool addPlatformEntry(QString platform);
    bool deletePlatformEntry(QString platform);
    bool incrementPlatformGameCounter(QString platform,bool plus);

    QStringList getAllPlatforms();
    QByteArray getPlatformImage(QString platform);
    QString getPlatformInfo(QString platform);
    int getPlatformGameCounter(QString platform);
    QString getPlatformEmulator(QString platform);
    QString getPlatformTimePlayed(QString platform);
    QString getPlatformLastPlayed(QString platform);
    int getPlatformOpenedCounter(QString platform);

    bool setPlatformEmulator(QString platform,QString emulatorName);
    bool setPlatformOpenedCounter(QString platform, int openedCounter);

    bool addEmulatorEntry(QString emulatorName,QString emulatorPath);
    bool deleteEmulatorEntry(QString emulatorName);
    QStringList getAllEmulators();
    QByteArray getEmulatorImage(QString emulatorName);
    QString getEmulatorInfo(QString emulatorName);
    QString getEmulatorPath(QString emulatorName);
    QString getEmulatorTimePlayed(QString emulatorName);
    QString getEmulatorLastPlayed(QString emulatorName);
    int getEmulatorOpenedCounter(QString emulatorName);

    bool setEmulatorPath(QString emulatorName,QString path);
    bool setEmulatorOpenedCounter(QString emulatorName, int openedCounter);

    QStringList getAllExternalSoftwares();
    bool addExternalSoftwareEntry(QString externalSoftwareName);
    bool deleteExternalSoftwareEntry(QString externalSoftwareName);
    bool checkAddExternalSoftwarePossible(QString externalSoftwareName);

    QString getExternalSoftwarePath(QString externalSoftwareName);
    bool getExternalSoftwareSupportedBool(QString externalSoftwareName);
    int getExternalSoftwareOpenedCounter(QString externalSoftwareName);
    bool getExternalSoftwareGameCanRun(QString extSoftName, int gameId);
    bool getExternalSoftwareMutliInstance(QString externalSoftwareName);

    QList<int> getExternalSoftwareGamesWhitelist(QString externalSoftwareName);
    QList<int> getExternalSoftwareGamesOffList(QString externalSoftwareName);
    QStringList getExternalSoftwarePlatformsOnList(QString externalSoftwareName);

    bool setExternalSoftwareGamesWhitelist(QString externalSoftwareName, QList<int> gameWhitelist);
    bool setExternalSoftwareGamesOffList(QString externalSoftwareName, QList<int> gamesOffList);
    bool setExternalSoftwarePlatformsOnList(QString externalSoftwareName, QStringList platformsOnList);
    bool setExternalSoftwareMutliInstance(QString externalSoftwareName, bool state);

    bool setExternalSoftwarePath(QString externalSoftwareName, QString path);
    bool setExternalSoftwareSupportedBool(QString externalSoftwareName, bool supported);
    bool setExternalSoftwareOpenedCounter(QString externalSoftwareName, int openedCounter);


private:
    QString m_db_name;
    QSqlDatabase m_db;

};

#endif // DBMANAGER_H
