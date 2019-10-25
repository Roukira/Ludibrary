#include "statsmonitor.h"

StatsMonitor::StatsMonitor(QObject *parent) : QObject(parent)
{
    QTimer *timer = new QTimer;
    m_timer = timer;
    m_timer->setInterval(m_timerInterval);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateStatistics()));
    m_timer->start();
}


void StatsMonitor::updateStatistics()
{
    DbManager db("data.db");

    updateGeneralStats();
    foreach(QString platform,db.getAllPlatforms())
    {
        if (platformStatsExist(platform))
        {
            updatePlatformStat(platform);
        }
        else
        {
            createPlatformStat(platform);
        }

    }
    foreach(QString emulatorName,db.getAllEmulators())
    {
        if (emulatorStatsExist(emulatorName))
        {
            updateEmulatorStat(emulatorName);
        }
        else
        {
            createEmulatorStat(emulatorName);
        }
    }
    foreach(QString extSoftName, db.getAllExternalSoftwares()){
        if (extSoftStatsExist(extSoftName)){
            updateExtSoftStat(extSoftName);
        }
        else{
            createExtSoftStat(extSoftName);
        }
    }


}

void StatsMonitor::createPlatformStat(QString platform)
{
    DbManager db("data.db");
    int gameCounter = 0;
    int timePlayed = 0;
    int openedCounter = db.getPlatformOpenedCounter(platform);

    foreach(int gameID,db.getAllGames())
    {
        if (db.getGamePlatform(gameID)==platform)
        {
            gameCounter++;
            timePlayed += db.getGameTimePlayed(gameID);
        }
    }

    QLabel *platformNameLabel = new QLabel("\n"+platform+"\n");
    QLabel *platformGameCounterLabel = new QLabel(convertGameCounterToStringForPlatform(gameCounter,platform));
    QLabel *platformTimePlayedLabel = new QLabel(convertTimeToString(timePlayed)+" played on "+platform+".");
    QLabel *platformOpenedCounterLabel = new QLabel(convertOpenedCounterToString(openedCounter));

    QString styleSheetLabel = "QLabel {\n		background : transparent;\n		color: black;\n		border-style: solid;\n		border-width: 0px;\n		border-radius: 5px;\n		border-color: rgb(202, 202, 202);\n}";
    platformNameLabel->setStyleSheet(styleSheetLabel);
    platformGameCounterLabel->setStyleSheet(styleSheetLabel);
    platformTimePlayedLabel->setStyleSheet(styleSheetLabel);
    platformOpenedCounterLabel->setStyleSheet(styleSheetLabel);

    QFont fontLabel("Arial",11);
    fontLabel.setBold(true);
    fontLabel.setKerning(true);

    QFont platformNameFont("Arial",14);
    platformNameFont.setBold(true);
    platformNameFont.setKerning(true);
    platformNameFont.setUnderline(true);

    platformNameLabel->setFont(platformNameFont);
    platformGameCounterLabel->setFont(fontLabel);
    platformTimePlayedLabel->setFont(fontLabel);
    platformOpenedCounterLabel->setFont(fontLabel);

    QList<QLabel*> list;
    list.append(platformNameLabel);
    list.append(platformGameCounterLabel);
    list.append(platformTimePlayedLabel);
    list.append(platformOpenedCounterLabel);

    m_platformList.append(list);
    m_platformListString.append(platform);

    emit createPlatformQuery(platformNameLabel,platformGameCounterLabel,platformTimePlayedLabel,platformOpenedCounterLabel);
}


void StatsMonitor::createEmulatorStat(QString emulatorName)
{
    DbManager db("data.db");
    int gameCounter = 0;
    int timePlayed = 0;
    int openedCounter = db.getEmulatorOpenedCounter(emulatorName);

    foreach(int gameID,db.getAllGames())
    {
        if (db.getGameEmulator(gameID)==emulatorName)
        {
            gameCounter++;
            timePlayed += db.getGameTimePlayed(gameID);
        }
    }

    QLabel *emulatorNameLabel = new QLabel("\n"+emulatorName+"\n");
    QLabel *emulatorGameCounterLabel = new QLabel(convertGameCounterToStringForEmulator(gameCounter,emulatorName));
    QLabel *emulatorTimePlayedLabel = new QLabel(convertTimeToString(timePlayed)+" played on "+emulatorName+".");
    QLabel *emulatorOpenedCounterLabel = new QLabel(convertOpenedCounterToString(openedCounter));

    QString styleSheetLabel = "QLabel {\n		background : transparent;\n		color: black;\n		border-style: solid;\n		border-width: 0px;\n		border-radius: 5px;\n		border-color: rgb(202, 202, 202);\n}";
    emulatorNameLabel->setStyleSheet(styleSheetLabel);
    emulatorGameCounterLabel->setStyleSheet(styleSheetLabel);
    emulatorTimePlayedLabel->setStyleSheet(styleSheetLabel);
    emulatorOpenedCounterLabel->setStyleSheet(styleSheetLabel);

    QFont emulatorNameFont("Arial",14);
    emulatorNameFont.setBold(true);
    emulatorNameFont.setKerning(true);
    emulatorNameFont.setUnderline(true);

    QFont fontLabel("Arial",11);
    fontLabel.setBold(true);
    fontLabel.setKerning(true);

    emulatorNameLabel->setFont(emulatorNameFont);
    emulatorGameCounterLabel->setFont(fontLabel);
    emulatorTimePlayedLabel->setFont(fontLabel);
    emulatorOpenedCounterLabel->setFont(fontLabel);

    QList<QLabel*> list;
    list.append(emulatorNameLabel);
    list.append(emulatorGameCounterLabel);
    list.append(emulatorTimePlayedLabel);
    list.append(emulatorOpenedCounterLabel);

    m_emulatorList.append(list);
    m_emulatorListString.append(emulatorName);

    emit createEmulatorQuery(emulatorNameLabel,emulatorGameCounterLabel,emulatorTimePlayedLabel,emulatorOpenedCounterLabel);

}

void StatsMonitor::createExtSoftStat(QString extSoftName)
{
    DbManager db("data.db");
    int openedCounter = db.getExternalSoftwareOpenedCounter(extSoftName);

    QLabel *extSoftNameLabel = new QLabel("\n"+extSoftName+"\n");
    QLabel *extSoftOpenedCounterLabel = new QLabel(convertOpenedCounterToString(openedCounter));

    QString styleSheetLabel = "QLabel {\n		background : transparent;\n		color: black;\n		border-style: solid;\n		border-width: 0px;\n		border-radius: 5px;\n		border-color: rgb(202, 202, 202);\n}";
    extSoftNameLabel->setStyleSheet(styleSheetLabel);
    extSoftOpenedCounterLabel->setStyleSheet(styleSheetLabel);

    QFont extSoftNameFont("Arial",14);
    extSoftNameFont.setBold(true);
    extSoftNameFont.setKerning(true);
    extSoftNameFont.setUnderline(true);

    QFont fontLabel("Arial",11);
    fontLabel.setBold(true);
    fontLabel.setKerning(true);

    extSoftNameLabel->setFont(extSoftNameFont);
    extSoftOpenedCounterLabel->setFont(fontLabel);

    QList<QLabel*> list;
    list.append(extSoftNameLabel);
    list.append(extSoftOpenedCounterLabel);

    m_extSoftList.append(list);
    m_extSoftListString.append(extSoftName);

    emit createExtSoftQuery(extSoftNameLabel,extSoftOpenedCounterLabel);

}

void StatsMonitor::updateGeneralStats()
{
    DbManager db("data.db");
    int timePlayed = 0;
    int gameCounter = 0;
    int platformCounter = 0;
    int emulatorCounter = 0;
    int extSoftCounter = 0;

    foreach(int gameID,db.getAllGames())
    {
        gameCounter++;
        timePlayed += db.getGameTimePlayed(gameID);
    }

    foreach(QString platform,db.getAllPlatforms())
    {
        platformCounter++;
    }

    foreach(QString emulator, db.getAllEmulators())
    {
        emulatorCounter++;
    }

    foreach(QString extSoft, db.getAllExternalSoftwares())
    {
        extSoftCounter++;
    }
    QString games = convertGameCounterToString(gameCounter);
    QString platforms = convertPlatformCounterToString(platformCounter);
    QString emulators = convertEmulatorCounterToString(emulatorCounter);
    QString extSofts = convertExtSoftCounterToString(extSoftCounter);
    QString totalPlayTime = convertTimeToString(timePlayed)+" played.";

    m_totalPlayTime = timePlayed;
    m_gameCounter = gameCounter;
    m_platformCounter = platformCounter;
    m_emulatorCounter = emulatorCounter;
    m_extSoftCounter = extSoftCounter;
    //use emit query to mainwindow.
    emit updateGeneralQuery(games,platforms,emulators,totalPlayTime, extSofts);
}

void StatsMonitor::updatePlatformStat(QString platform)
{
    DbManager db("data.db");

    QLabel *platformNameLabel;
    QLabel *platformGameCounterLabel;
    QLabel *platformTimePlayedLabel;
    QLabel *platformOpenedCounterLabel;

    int gameCounter = 0;
    int timePlayed = 0;
    int openedCounter = db.getPlatformOpenedCounter(platform);

    foreach (QList<QLabel*> list,m_platformList)
    {
        platformNameLabel = list.at(0);
        if(platformNameLabel->text()==platform)
        {
            platformGameCounterLabel = list.at(1);
            platformTimePlayedLabel = list.at(2);
            platformOpenedCounterLabel = list.at(3);

            foreach(int gameID,db.getAllGames())
            {
                if (db.getGamePlatform(gameID)==platform)
                {
                    gameCounter++;
                    timePlayed += db.getGameTimePlayed(gameID);
                }
            }

            platformGameCounterLabel->setText(convertGameCounterToStringForPlatform(gameCounter,platform));
            platformTimePlayedLabel->setText(convertTimeToString(timePlayed)+" played on "+platform+".");
            platformOpenedCounterLabel->setText(convertOpenedCounterToString(openedCounter));

            return;
        }
    }
}

void StatsMonitor::updateEmulatorStat(QString emulatorName)
{
    DbManager db("data.db");

    QLabel *emulatorNameLabel;
    QLabel *emulatorGameCounterLabel;
    QLabel *emulatorTimePlayedLabel;
    QLabel *emulatorOpenedCounterLabel;

    int gameCounter = 0;
    int timePlayed = 0;
    int openedCounter = db.getEmulatorOpenedCounter(emulatorName);

    foreach (QList<QLabel*> list,m_emulatorList)
    {
        emulatorNameLabel = list.at(0);
        if(emulatorNameLabel->text()==emulatorName)
        {
            emulatorGameCounterLabel = list.at(1);
            emulatorTimePlayedLabel = list.at(2);
            emulatorOpenedCounterLabel = list.at(3);

            foreach(int gameID,db.getAllGames())
            {
                if (db.getGameEmulator(gameID)==emulatorName)
                {
                    gameCounter++;
                    timePlayed += db.getGameTimePlayed(gameID);
                }
            }

            emulatorGameCounterLabel->setText(convertGameCounterToStringForEmulator(gameCounter,emulatorName));
            emulatorTimePlayedLabel->setText(convertTimeToString(timePlayed)+" played on "+emulatorName+".");
            emulatorOpenedCounterLabel->setText(convertOpenedCounterToString(openedCounter));

            return;
        }
    }

}

void StatsMonitor::updateExtSoftStat(QString extSoftName)
{
    DbManager db("data.db");

    QLabel *extSoftNameLabel;
    QLabel *extSoftOpenedCounterLabel;

    int openedCounter = db.getExternalSoftwareOpenedCounter(extSoftName);

    foreach (QList<QLabel*> list,m_extSoftList)
    {
        extSoftNameLabel = list.at(0);
        if(extSoftNameLabel->text()==extSoftName)
        {
            extSoftOpenedCounterLabel = list.at(1);

            extSoftOpenedCounterLabel->setText(convertOpenedCounterToString(openedCounter));
            return;
        }
    }

}
bool StatsMonitor::platformStatsExist(QString platform)
{
    return m_platformListString.contains(platform);
}

bool StatsMonitor::emulatorStatsExist(QString emulatorName)
{
    return m_emulatorListString.contains(emulatorName);
}

bool StatsMonitor::extSoftStatsExist(QString extSoftName){
    return m_extSoftListString.contains(extSoftName);
}

QString StatsMonitor::convertGameCounterToString(int gameCounter)
{
    if(gameCounter==1)
    {
        return "\n"+QString::number(gameCounter)+" game owned.";
    }
    return "\n"+QString::number(gameCounter)+" games owned.";
}

QString StatsMonitor::convertExtSoftCounterToString(int extSoftCounter){
    if(extSoftCounter==1)
    {
        return QString::number(extSoftCounter)+" external software installed.";
    }
    return QString::number(extSoftCounter)+" external softwares installed.";
}

QString StatsMonitor::convertGameCounterToStringForPlatform(int gameCounter, QString platform)
{
    if(gameCounter==1)
    {
        return QString::number(gameCounter)+" "+platform+" game owned.";
    }
    return QString::number(gameCounter)+" "+platform+" games owned.";
}

QString StatsMonitor::convertGameCounterToStringForEmulator(int gameCounter, QString emulatorName)
{
    if(gameCounter==1)
    {
        return QString::number(gameCounter)+" game played through "+emulatorName+".";
    }
    return QString::number(gameCounter)+" games played through "+emulatorName+".";
}


QString StatsMonitor::convertOpenedCounterToString(int openedCounter){
    if(openedCounter==1){
        return QString::number(openedCounter)+" time opened.";
    }
    return QString::number(openedCounter)+" times opened.";
}

QString StatsMonitor::convertPlatformCounterToString(int platformCounter)
{
    if(platformCounter==1)
    {
        return QString::number(platformCounter)+" platform used.";
    }
    return QString::number(platformCounter)+" platforms used.";
}


QString StatsMonitor::convertEmulatorCounterToString(int emulatorCounter)
{
    if(emulatorCounter==1)
    {
        return QString::number(emulatorCounter)+" emulator installed.";
    }
    return QString::number(emulatorCounter)+" emulators installed.";
}

QString StatsMonitor::convertTimeToString(int timePlayed)
{
    if(timePlayed<60)
    {
        if(timePlayed==1)
        {
            return "1 second";
        }
        return QString::number(timePlayed)+" seconds";
    }
    else if((timePlayed/60)<60)
    {
        if((timePlayed/60)==1)
        {
            return "1 minute";
        }
        return QString::number(timePlayed/60)+" minutes";
    }
    else
    {
        if((timePlayed/3600)==1)
        {
            return "1 hour";
        }
        return QString::number(timePlayed/3600)+" hours";
    }
}

void StatsMonitor::deletePlatformStat(QString platform)
{
    int i;
    for (i=0;i<m_platformListString.length();i++)
    {
        if(m_platformListString.at(i)==platform)
        {
            foreach(QLabel* label,m_platformList.at(i))
            {
                label->deleteLater();
            }
            m_platformList.removeAt(i);
            m_platformListString.removeAt(i);
            updateStatistics();
            return;
        }
    }
}

void StatsMonitor::deleteEmulatorStat(QString emulatorName)
{
    int i;
    for (i=0;i<m_emulatorListString.length();i++)
    {
        if(m_emulatorListString.at(i)==emulatorName)
        {
            foreach(QLabel* label,m_emulatorList.at(i))
            {
                label->deleteLater();
            }
            m_emulatorList.removeAt(i);
            m_emulatorListString.removeAt(i);
            updateStatistics();
            return;
        }
    }
}

void StatsMonitor::deleteExtSoftStat(QString extSoftName){
    int i;
    for (i=0;i<m_extSoftListString.length();i++){
        if(m_extSoftListString.at(i)==extSoftName){
            foreach(QLabel* label, m_extSoftList.at(i)){
                label->deleteLater();
            }
            m_extSoftList.removeAt(i);
            m_extSoftListString.removeAt(i);
            updateStatistics();
            return;
        }
    }
}
