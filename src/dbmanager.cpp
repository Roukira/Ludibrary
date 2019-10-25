#include "dbmanager.h"




//=============================================================================================================================
//=================================================CONSTUCTOR==================================================================
//=============================================================================================================================



DbManager::DbManager(const QString& db_name)

/*Provided the database path db_name, it constructs the database and creates the file if it doesnt exist according
to these standards :
    - Table game containing games
    - Table platform containing platforms
    - Table emulator containing emulators
*/

{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.connectionName()=="")
    {
        initDb(db_name);
    }
    else
    {
        m_db = db;
        qDebug(qUtf8Printable("Db:nice"));
    }
}


void DbManager::initDb(const QString& db_name)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(db_name);
    m_db.open();
    m_db_name = db_name;
    QSqlQuery query1;
    query1.exec("create table if not exists game ("
                       "id integer primary key, "
                       "game_title varchar(1000), "
                       "platform varchar(1000), "
                       "emulator_name varchar(1000), "
                       "path varchar(1000), "
                       "release_date varchar(100), "
                       "image BLOB, "
                       "game_info text, "
                       "time_played integer, "
                       "last_played text, "
                       "game_favorited integer, "
                       "state text, "
                       "game_steam integer,"
                       "steamid integer,"
                       "opened_counter integer)");

     QSqlQuery query2;
     query2.exec("create table if not exists platform ("
                       "platform varchar(1000) primary key, "
                       "emulator_name varchar(1000), "
                       "image BLOB, "
                       "platform_info text, "
                       "time_played integer, "
                       "last_played text,"
                       "game_counter integer,"
                       "opened_counter integer)");

     QSqlQuery query3;
     query3.exec("create table if not exists emulator ("
                       "emulator_name varchar(1000) primary key, "
                       "image BLOB, "
                       "path varchar(1000), "
                       "emulator_info text, "
                       "time_played integer, "
                       "last_played text,"
                       "opened_counter integer)");

     QSqlQuery query4;
     query4.exec("create table if not exists extsoft ("
                 "software_name varchar(1000) primary key,"
                 "image BLOB,"
                 "path varchar(1000),"
                 "supported_extsoft integer,"
                 "multi_instance integer,"
                 "opened_counter integer,"
                 "games_off_list text,"
                 "games_whitelist text,"
                 "platforms_on_list text)");
}


void DbManager::close()
{
    m_db.close();
}

void DbManager::setDatabaseName(const QString& path)
{
    m_db.setDatabaseName(path);
}

bool DbManager::checkAddGamePossible(int id)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT EXISTS(SELECT 1 FROM game WHERE id=:id LIMIT 1);");
    query.bindValue(":id",id);

    if(query.exec())
    {
        while (query.next())
        {
            success = query.value(0).toBool();
            success = !success;
        }
    }
    else
    {
        qDebug() << "checkAddGamePossible error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::checkAddExternalSoftwarePossible(QString externalSoftwareName)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("SELECT EXISTS(SELECT 1 FROM extsoft WHERE software_name=:software_name LIMIT 1);");
    query.bindValue(":software_name",externalSoftwareName);

    if(query.exec())
    {
        while (query.next())
        {
            success = query.value(0).toBool();
            success = !success;
        }
    }
    else
    {
        qDebug() << "checkAddExternalSoftwareEntry error:  " << query.lastError().text();
    }
    return success;
}

//=============================================================================================================================
//=================================================GAME TABLE OPERATIONS=======================================================
//=============================================================================================================================



bool DbManager::addGameEntry(int id, QString gameTitle, QString platform, int steamid)

/*Provided a game, this function adds it to the game table.
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO game (id,game_title,platform,time_played,steamid,opened_counter,state,last_played) VALUES(:id,:game_title,:platform,:time_played,:steamid,:opened_counter,:state,:last_played)");
    query.bindValue(":id",id);
    query.bindValue(":game_title",gameTitle);
    query.bindValue(":platform",platform);
    query.bindValue(":time_played",0);
    query.bindValue(":steamid",steamid);
    query.bindValue(":opened_counter",0);
    query.bindValue(":state","");
    query.bindValue(":last_played","");
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "addGameEntry error:  " << query.lastError().text();
    }
    return success;
}


bool DbManager::deleteGameEntry(int id)

/*Provided a game id, this function removes it from the game table(and therefore from the database).
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM game WHERE id = :id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "deleteGameEntry error:  " << query.lastError().text();
    }
    return success;
}

//=============================================================================================================================
//=================================================GET OPERATIONS==============================================================
//=============================================================================================================================

QList<int> DbManager::getAllGames()
{
    QList<int> AllGames;
    QSqlQuery query;
    query.prepare("SELECT id FROM game;");
    if(query.exec())
    {
        while (query.next())
        {
            AllGames.append(query.value(0).toInt());
        }
    }
    else
    {
        qDebug() << "getAllGames error:  " << query.lastError().text();
    }
    return AllGames;
}

int DbManager::getGameId(QString gameTitle, QString platform)

/*Provided a game_title and platform, this function returns its id.
*/

{
    int id(-1);
    QSqlQuery query;
    query.prepare("SELECT id FROM game WHERE game_title=:game_title AND platform=:platform;");
    query.bindValue(":game_title",gameTitle);
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            id = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(id)));
        }
    }
    else
    {
        qDebug() << "getGameId error:  " << query.lastError().text();
    }
    return id;
}

int DbManager::getGameSteamId(QString gameTitle, QString platform)
{
/*Provided a game_title and platform, this function returns its id.
*/
    int steamid(-1);
    QSqlQuery query;
    query.prepare("SELECT steamid FROM game WHERE game_title=:game_title AND platform=:platform;");
    query.bindValue(":game_title",gameTitle);
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            steamid = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(steamid)));
        }
    }
    else
    {
        qDebug() << "getGameSteamId error:  " << query.lastError().text();
    }
    return steamid;
}

QString DbManager::getGameTitle(int id)

/*Provided a game id, this function returns its game_title.
*/

{
    QString gameTitle("");
    QSqlQuery query;
    query.prepare("SELECT game_title FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            gameTitle = query.value(0).toString();
            qDebug(qUtf8Printable(gameTitle));
        }
    }
    else
    {
        qDebug() << "getGameTitle error:  " << query.lastError().text();
    }
    return gameTitle;
}


QString DbManager::getGamePlatform(int id)

/*Provided a game id, this function returns its platform.
*/

{
    QString platform("");
    QSqlQuery query;
    query.prepare("SELECT platform FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            platform = query.value(0).toString();
            qDebug(qUtf8Printable(platform));
        }
    }
    else
    {
        qDebug() << "getGamePlatform error:  " << query.lastError().text();
    }
    return platform;
}


QString DbManager::getGamePath(int id)

/*Provided a game id, this function returns its path.
*/

{
    QString path("");
    QSqlQuery query;
    query.prepare("SELECT path FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            path = query.value(0).toString();
            qDebug(qUtf8Printable(path));
        }
    }
    else
    {
        qDebug() << "getGamePath error:  " << query.lastError().text();
    }
    return path;
}


QByteArray DbManager::getGameImage(int id)

/*Provided a game id, this function returns its image byte array.
*/

{
    QByteArray gameImage;
    QSqlQuery query;
    query.prepare("SELECT image FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            gameImage = query.value(0).toByteArray();
        }
    }
    else
    {
        qDebug() << "getGameImage error:  " << query.lastError().text();
    }
    return gameImage;
}


QString DbManager::getGameInfo(int id)

/*Provided a game id, this function returns its game_info.
*/

{
    QString game_info("");
    QSqlQuery query;
    query.prepare("SELECT game_info FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            game_info = query.value(0).toString();
            qDebug(qUtf8Printable(game_info));
        }
    }
    else
    {
        qDebug() << "getGameInfo error:  " << query.lastError().text();
    }
    return game_info;
}


QString DbManager::getGameEmulator(int id)

/*Provided a game id, this function returns its emulator_name.
*/

{
    QString emulatorName("");
    QSqlQuery query;
    query.prepare("SELECT emulator_name FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            emulatorName = query.value(0).toString();
            qDebug(qUtf8Printable(emulatorName));
        }
    }
    else
    {
        qDebug() << "getGameEmulator error:  " << query.lastError().text();
    }
    return emulatorName;
}


QString DbManager::getGameReleaseDate(int id)

/*Provided a game id, this function returns its release_date.
*/

{
    QString releaseDate("");
    QSqlQuery query;
    query.prepare("SELECT release_date FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            releaseDate = query.value(0).toString();
            qDebug(qUtf8Printable(releaseDate));
        }
    }
    else
    {
        qDebug() << "getGameReleaseDate error:  " << query.lastError().text();
    }
    return releaseDate;
}


int DbManager::getGameTimePlayed(int id)

/*Provided a game id, this function returns its time_played.
*/

{
    int time = -1;
    QSqlQuery query;
    query.prepare("SELECT time_played FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            time = query.value(0).toInt();
            qDebug(qUtf8Printable("time played on database : "+QString::number(time)));
        }
    }
    else
    {
        qDebug() << "getGameTimePlayed error:  " << query.lastError().text();
    }
    return time;

}


QString DbManager::getGameLastPlayed(int id)

/*Provided a game id, this function returns its last_played.
*/

{
    QString lastPlayed("Never Played");
    QSqlQuery query;
    query.prepare("SELECT last_played FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            lastPlayed = query.value(0).toString();
            qDebug(qUtf8Printable(lastPlayed));
        }
    }
    else
    {
        qDebug() << "getGameLastPlayed error:  " << query.lastError().text();
    }
    qDebug(qUtf8Printable("debug20"));
    return lastPlayed;
}

int DbManager::getGameOpenedCounter(int id)
{
    int openedCounter=-1;
    QSqlQuery query;
    query.prepare("SELECT opened_counter FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            openedCounter = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(openedCounter)));
        }
    }
    else
    {
        qDebug() << "getGameOpenedCounter error: " << query.lastError().text();
    }
    return openedCounter;
}


QString DbManager::getGameState(int id)
{
    QString state("");
    QSqlQuery query;
    query.prepare("SELECT state FROM game WHERE id=:id;");
    query.bindValue(":id",id);
    if(query.exec())
    {
        while (query.next())
        {
            state = query.value(0).toString();
        }
    }
    else
    {
        qDebug() << "getGameState error: " << query.lastError().text();
    }
    return state;
}

QDateTime DbManager::stringToDate(QString dateString)
{
    qDebug(qUtf8Printable("debug21"));
    if (dateString==""||dateString=="Never Played")
    {
        return QDateTime();
    }
    qDebug(qUtf8Printable(dateString));
    QStringList yyyymmddPLUShhmmss = dateString.split(" ");
    qDebug(qUtf8Printable("debug22"));
    QString yyyymmddstring = yyyymmddPLUShhmmss.at(0);
    QString hhmmssstring = yyyymmddPLUShhmmss.at(1);
    qDebug(qUtf8Printable("debug23"));
    QStringList yyyymmdd = yyyymmddstring.split("-");
    QStringList hhmmss = hhmmssstring.split(":");
    qDebug(qUtf8Printable("debug24"));
    QDateTime date;
    QString year = yyyymmdd.at(0);
    qDebug(qUtf8Printable("debug30"));
    QString month = yyyymmdd.at(1);
    qDebug(qUtf8Printable("debug25"));
    QString days = yyyymmdd.at(2);
    qDebug(qUtf8Printable("debug26"));
    QString hours = hhmmss.at(0);
    QString minutes = hhmmss.at(1);
    qDebug(qUtf8Printable("debug27"));
    QString seconds = hhmmss.at(2);
    qDebug(qUtf8Printable("debug28"));
    date.setDate(QDate(year.toInt(),month.toInt(),days.toInt()));
    date.setTime(QTime(hours.toInt(),minutes.toInt(),seconds.toInt()));
    return date;

}

//=============================================================================================================================
//=================================================SET OPERATIONS==============================================================
//=============================================================================================================================

bool DbManager::setGameImage(int id, QByteArray imageData)
//Provided a game id and image data, this function sets the game image.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET image=:image WHERE id=:id;");
    query.bindValue(":image",imageData);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGameImage error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setGameInfo(int id, QString gameInfo)
//Provided a game id and game_info, this function sets the game_info.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET game_info=:game_info WHERE id=:id;");
    query.bindValue(":game_info",gameInfo);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGameInfo error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setGamePath(int id, QString gamePath)
//Provided a game id and game_path, this function sets the game_path.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET path=:path WHERE id=:id;");
    query.bindValue(":path",gamePath);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGamePath error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setGameTimePlayed(int id, int timePlayed)
//Provided a game id and timePlayed, this function sets the time_played.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET time_played=:time_played WHERE id=:id;");
    query.bindValue(":time_played",timePlayed);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGameTimePlayed error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::updateGameLastPlayed(int id)
{
    bool success = false;
    QString date("");
    QSqlQuery query1,query2;
    query1.prepare("SELECT DATETIME('now','localtime');");
    query2.prepare("UPDATE game SET last_played=:last_played WHERE id=:id;");
    if(query1.exec())
    {
        while(query1.next())
        {
            date = query1.value(0).toString();
        }
    }
    else
    {
        qDebug() << "updateGameLastPlayed error : " << query1.lastError().text();
        return success;
    }
    if (date == "")
    {
        return success;
    }
    query2.bindValue(":last_played",date);
    query2.bindValue(":id",id);
    if(query2.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "updateGameLastPlayed error : " << query2.lastError().text();
    }
    return success;
}

bool DbManager::setGameState(int id, QString state)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET state=:state WHERE id=:id;");
    query.bindValue(":state",state);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGameState error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setGameOpenedCounter(int id, int openedCounter)
//Provided a game id and timePlayed, this function sets the time_played.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE game SET opened_counter=:opened_counter WHERE id=:id;");
    query.bindValue(":opened_counter",openedCounter);
    query.bindValue(":id",id);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setGameOpenedCounter error:  " << query.lastError().text();
    }
    return success;
}


//=============================================================================================================================
//=================================================PLATFORM TABLE OPERATIONS===================================================
//=============================================================================================================================

QString DbManager::PlatformToSupportedEmulator(QString platform)
{
    if (platform=="Nintendo Entertainment System (NES)")
    {
        return "nestopia";
    }
    else if (platform=="Super Nintendo (SNES)")
    {
        return "snes9x";
    }
    else if (platform=="Nintendo 64")
    {
        return "Project64";
    }
    else if (platform=="Nintendo GameCube"||platform=="Nintendo Wii")
    {
        return "Dolphin";
    }
    else if (platform=="Nintendo Wii U")
    {
        return "Cemu";
    }
    else if (platform=="Nintendo Game Boy"||platform=="Nintendo Game Boy Color"||platform =="Nintendo Game Boy Advance")
    {
        return "VisualBoyAdvance";
    }
    else if (platform=="Nintendo DS")
    {
        return "DeSmuME";
    }
    else if (platform=="Nintendo 3DS")
    {
        return "citra";
    }
    else if (platform=="Sony Playstation")
    {
        return "ePSXe";
    }
    else if (platform=="Sony Playstation 2")
    {
        return "pcsx2";
    }
    else if (platform=="Sony Playstation 3")
    {
        return "rpcs3";
    }
    else if (platform=="Sony Playstation Portable")
    {
        return "PPSSPP";
    }
    else if (platform=="PC")
    {
        return "PC";
    }
    return platform;
}

QStringList DbManager::SupportedEmulatorToPlatforms(QString emulatorName)
{
    QStringList platforms;
    if (emulatorName=="nestopia")
    {
        platforms.append("Nintendo Entertainment System (NES)");
    }
    else if (emulatorName=="snes9x")
    {
        platforms.append("Super Nintendo (SNES)");
    }
    else if (emulatorName=="Project64")
    {
        platforms.append("Nintendo 64");
    }
    else if (emulatorName=="Dolphin")
    {
        platforms.append("Nintendo GameCube");
        platforms.append("Nintendo Wii");
    }
    else if (emulatorName=="Cemu")
    {
        platforms.append("Nintendo Wii U");
    }
    else if (emulatorName=="VisualBoyAdvance")
    {
        platforms.append("Nintendo Game Boy");
        platforms.append("Nintendo Game Boy Color");
        platforms.append("Nintendo Game Boy Advance");
    }
    else if (emulatorName=="DeSmuME")
    {
        platforms.append("Nintendo DS");
    }
    else if (emulatorName=="citra")
    {
        platforms.append("Nintendo 3DS");
    }
    else if (emulatorName=="ePSXe")
    {
        platforms.append("Sony Playstation");
    }
    else if (emulatorName=="pcsx2")
    {
        platforms.append("Sony Playstation 2");
    }
    else if (emulatorName=="rpcs3")
    {
        platforms.append("Sony Playstation 3");
    }
    else if (emulatorName=="PPSSPP")
    {
        platforms.append("Sony Playstation Portable");
    }
    return platforms;
}

bool DbManager::addPlatformEntry(QString platform)

/*Provided a platform, this function adds it to the platform table.
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO platform (platform,game_counter,opened_counter) VALUES (:platform,:game_counter,:opened_counter)");
    query.bindValue(":platform",platform);
    query.bindValue(":game_counter",0);
    query.bindValue(":opened_counter",0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "addPlatformEntry error:  " << query.lastError().text();
    }
    setPlatformEmulator(platform,PlatformToSupportedEmulator(platform));
    return success;
}


bool DbManager::deletePlatformEntry(QString platform)

/*Provided a platform name, this function removes it from the platform table(and therefore from the database).
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM platform WHERE platform = :platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "deletePlatformEntry error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::incrementPlatformGameCounter(QString platform, bool plus)
//Provided a platform, increments its game_counter by 1.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE platform SET game_counter=:game_counter WHERE platform=:platform;");
    if (plus)
    {
        query.bindValue(":game_counter",getPlatformGameCounter(platform)+1);
    }
    else
    {
        query.bindValue(":game_counter",getPlatformGameCounter(platform)-1);
    }
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "incrementPlatformGameCounter error:  " << query.lastError().text();
    }
    return success;
}


QStringList DbManager::getAllPlatforms()
{
    QStringList allPlatforms;
    QSqlQuery query;
    query.prepare("SELECT platform FROM platform;");
    if(query.exec())
    {
        while (query.next())
        {
            allPlatforms.append(query.value(0).toString());
        }
    }
    else
    {
        qDebug() << "getAllPlatforms error:  " << query.lastError().text();
    }
    return allPlatforms;
}


QByteArray DbManager::getPlatformImage(QString platform)

/*Provided a platform name, this function returns its image byte array.
*/

{
    QByteArray PlatformImage;
    QSqlQuery query;
    query.prepare("SELECT image_path FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            PlatformImage = query.value(0).toByteArray();
        }
    }
    else
    {
        qDebug() << "getPlatformImage error:  " << query.lastError().text();
    }
    return PlatformImage;
}


QString DbManager::getPlatformInfo(QString platform)

/*Provided a platform name, this function returns its platform_info.
*/

{
    QString PlatformInfo("");
    QSqlQuery query;
    query.prepare("SELECT platform_info FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            PlatformInfo = query.value(0).toString();
            qDebug(qUtf8Printable(PlatformInfo));
        }
    }
    else
    {
        qDebug() << "getPlatformInfo error:  " << query.lastError().text();
    }
    return PlatformInfo;
}

int DbManager::getPlatformGameCounter(QString platform)

/*Provided a platform name, this function returns its game_counter.
*/

{
    int gameCounter;
    QSqlQuery query;
    query.prepare("SELECT game_counter FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            gameCounter = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(gameCounter)));
        }
    }
    else
    {
        qDebug() << "getPlatformGameCounter error:  " << query.lastError().text();
    }
    return gameCounter;
}


QString DbManager::getPlatformEmulator(QString platform)

/*Provided a platform name, this function returns its emulator_name.
*/

{
    QString PlatformEmulator("");
    QSqlQuery query;
    query.prepare("SELECT emulator_name FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            PlatformEmulator = query.value(0).toString();
            qDebug(qUtf8Printable(PlatformEmulator));
        }
    }
    else
    {
        qDebug() << "getPlatformEmulator error:  " << query.lastError().text();
    }
    return PlatformEmulator;
}


QString DbManager::getPlatformTimePlayed(QString platform)

/*Provided a platform name, this function returns its time_played.
*/

{
    QString timePlayed("");
    QSqlQuery query;
    query.prepare("SELECT time_played FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            timePlayed = query.value(0).toString();
            qDebug(qUtf8Printable(timePlayed));
        }
    }
    else
    {
        qDebug() << "getPlatformTimePlayed error:  " << query.lastError().text();
    }
    return timePlayed;
}


QString DbManager::getPlatformLastPlayed(QString platform)

/*Provided a platform name, this function returns its last_played.
*/

{
    QString lastPlayed("");
    QSqlQuery query;
    query.prepare("SELECT last_played FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            lastPlayed = query.value(0).toString();
            qDebug(qUtf8Printable(lastPlayed));
        }
    }
    else
    {
        qDebug() << "getPlatformLastPlayed error:  " << query.lastError().text();
    }
    return lastPlayed;
}

int DbManager::getPlatformOpenedCounter(QString platform)
{
    int openedCounter=-1;
    QSqlQuery query;
    query.prepare("SELECT opened_counter FROM platform WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        while (query.next())
        {
            openedCounter = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(openedCounter)));
        }
    }
    else
    {
        qDebug() << "getPlatformOpenedCounter error: " << query.lastError().text();
    }
    return openedCounter;
}

bool DbManager::setPlatformEmulator(QString platform, QString emulatorName)
//Provided a platform and an emulator_name, this function sets it.
{
    if (platform=="PC")
    {
        return true;
    }
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE platform SET emulator_name=:emulator_name WHERE platform=:platform;");
    query.bindValue(":platform",platform);
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setPlatformEmulator error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setPlatformOpenedCounter(QString platform, int openedCounter)
//Provided a game id and timePlayed, this function sets the time_played.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE platform SET opened_counter=:opened_counter WHERE platform=:platform;");
    query.bindValue(":opened_counter",openedCounter);
    query.bindValue(":platform",platform);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setPlatformOpenedCounter error:  " << query.lastError().text();
    }
    return success;
}

//=============================================================================================================================
//=================================================EMULATOR TABLE OPERATIONS===================================================
//=============================================================================================================================



bool DbManager::addEmulatorEntry(QString emulatorName,QString emulatorPath)

/*Provided an emulator, this function adds it to the emulator table.
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO emulator (emulator_name,path,opened_counter) VALUES (:emulator_name,:path,:opened_counter)");
    query.bindValue(":emulator_name",emulatorName);
    query.bindValue(":path",emulatorPath);
    query.bindValue(":opened_counter",0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "addEmulatorEntry error:  " << query.lastError().text();
    }
    return success;
}


bool DbManager::deleteEmulatorEntry(QString emulatorName)

/*Provided an emulator name, this function removes it from the emulator table(and therefore from the database).
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM emulator WHERE emulator_name = :emulator_name;");
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "deleteEmulatorEntry error:  " << query.lastError().text();
    }
    return success;
}


QByteArray DbManager::getEmulatorImage(QString emulatorName)
{

    /*Provided a emulator_name, this function returns its image byte array.
    */

    {
        QByteArray EmulatorImage;
        QSqlQuery query;
        query.prepare("SELECT image_path FROM emulator WHERE emulator_name=:emulator_name;");
        query.bindValue(":emulator_name",emulatorName);
        if(query.exec())
        {
            while (query.next())
            {
                EmulatorImage = query.value(0).toByteArray();
            }
        }
        else
        {
            qDebug() << "getEmulatorImage error:  " << query.lastError().text();
        }
        return EmulatorImage;
    }


}


QString DbManager::getEmulatorInfo(QString emulatorName)
{

    /*Provided a emulator_name, this function returns its emulator_info.
    */

    {
        QString EmulatorInfo("");
        QSqlQuery query;
        query.prepare("SELECT emulator_info FROM emulator WHERE emulator_name=:emulator_name;");
        query.bindValue(":emulator_name",emulatorName);
        if(query.exec())
        {
            while (query.next())
            {
                EmulatorInfo = query.value(0).toString();
                qDebug(qUtf8Printable(EmulatorInfo));
            }
        }
        else
        {
            qDebug() << "getEmulatorInfo error:  " << query.lastError().text();
        }
        return EmulatorInfo;
    }


}


QString DbManager::getEmulatorPath(QString emulatorName)
/*Provided a emulator_name, this function returns its path.
*/
{
    QString EmulatorPath("");
    QSqlQuery query;
    query.prepare("SELECT path FROM emulator WHERE emulator_name=:emulator_name;");
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        while (query.next())
        {
            EmulatorPath = query.value(0).toString();
            qDebug(qUtf8Printable(EmulatorPath));
        }
    }
    else
    {
        qDebug() << "getEmulatorPath error:  " << query.lastError().text();
    }
    return EmulatorPath;
}


QString DbManager::getEmulatorTimePlayed(QString emulatorName)

/*Provided a emulator_name, this function returns its timeplayed.
*/

{
    QString timePlayed("");
    QSqlQuery query;
    query.prepare("SELECT time_played FROM emulator WHERE emulator_name=:emulator_name;");
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        while (query.next())
        {
            timePlayed = query.value(0).toString();
            qDebug(qUtf8Printable(timePlayed));
        }
    }
    else
    {
        qDebug() << "getEmulatorTimePlayed error:  " << query.lastError().text();
    }
    return timePlayed;
}


QString DbManager::getEmulatorLastPlayed(QString emulatorName)

/*Provided a emulator_name, this function returns its last_played.
*/

{
    QString lastPlayed("");
    QSqlQuery query;
    query.prepare("SELECT last_played FROM emulator WHERE emulator_name=:emulator_name;");
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        while (query.next())
        {
            lastPlayed = query.value(0).toString();
            qDebug(qUtf8Printable(lastPlayed));
        }
    }
    else
    {
        qDebug() << "getEmulatorLastPlayed error:  " << query.lastError().text();
    }
    return lastPlayed;
}

QStringList DbManager::getAllEmulators()
{
    QStringList allEmulators;
    QSqlQuery query;
    query.prepare("SELECT emulator_name FROM emulator;");
    if(query.exec())
    {
        while (query.next())
        {
            allEmulators.append(query.value(0).toString());
        }
    }
    else
    {
        qDebug() << "getAllEmulators error:  " << query.lastError().text();
    }
    return allEmulators;
}

int DbManager::getEmulatorOpenedCounter(QString emulatorName)
{
    int openedCounter=-1;
    QSqlQuery query;
    query.prepare("SELECT opened_counter FROM emulator WHERE emulator_name=:emulator_name;");
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        while (query.next())
        {
            openedCounter = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(openedCounter)));
        }
    }
    else
    {
        qDebug() << "getEmulatorOpenedCounter error: " << query.lastError().text();
    }
    return openedCounter;
}

bool DbManager::setEmulatorPath(QString emulatorName, QString path)
//Provided an emulator_name and its path, this function sets it.
{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE emulator SET path=:path WHERE emulator_name=:emulator_name");
    query.bindValue(":path",path);
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setEmulatorPath error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setEmulatorOpenedCounter(QString emulatorName, int openedCounter)
//Provided a game id and timePlayed, this function sets the time_played.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE emulator SET opened_counter=:opened_counter WHERE emulator_name=:emulator_name;");
    query.bindValue(":opened_counter",openedCounter);
    query.bindValue(":emulator_name",emulatorName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setEmulatorOpenedCounter error:  " << query.lastError().text();
    }
    return success;
}


bool DbManager::addExternalSoftwareEntry(QString externalSoftwareName)

/*Provided an emulator, this function adds it to the emulator table.
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO extsoft (software_name,opened_counter) VALUES (:software_name,:opened_counter)");
    query.bindValue(":software_name",externalSoftwareName);
    query.bindValue(":opened_counter",0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "addExternalSoftwareEntry error:  " << query.lastError().text();
    }
    return success;
}


bool DbManager::deleteExternalSoftwareEntry(QString externalSoftwareName)

/*Provided an emulator name, this function removes it from the emulator table(and therefore from the database).
*/

{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM extsoft WHERE software_name = :software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "deleteExternalSoftwareEntry error:  " << query.lastError().text();
    }
    return success;
}

QStringList DbManager::getAllExternalSoftwares()
{
    QStringList allSoftwares;
    QSqlQuery query;
    query.prepare("SELECT software_name FROM extsoft;");
    if(query.exec())
    {
        while (query.next())
        {
            allSoftwares.append(query.value(0).toString());
        }
    }
    else
    {
        qDebug() << "getAllExternalSoftwares error:  " << query.lastError().text();
    }
    return allSoftwares;
}


QString DbManager::getExternalSoftwarePath(QString externalSoftwareName)

/*Provided a game id, this function returns its path.
*/

{
    QString path("");
    QSqlQuery query;
    query.prepare("SELECT path FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while (query.next())
        {
            path = query.value(0).toString();
            qDebug(qUtf8Printable(path));
        }
    }
    else
    {
        qDebug() << "getExternalSoftwarePath error:  " << query.lastError().text();
    }
    return path;
}

bool DbManager::getExternalSoftwareSupportedBool(QString externalSoftwareName)
{
    bool supported = false;
    QSqlQuery query;
    query.prepare("SELECT supported_extsoft FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while(query.next())
        {
            supported = query.value(0).toBool();
            qDebug(qUtf8Printable(QString::number(supported)));
        }
    }
    else
    {
        qDebug() << "getExternalSoftwareSupportedBool error : " << query.lastError().text();
    }
    return supported;
}

int DbManager::getExternalSoftwareOpenedCounter(QString externalSoftwareName)
{
    int openedCounter=-1;
    QSqlQuery query;
    query.prepare("SELECT opened_counter FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while (query.next())
        {
            openedCounter = query.value(0).toInt();
            qDebug(qUtf8Printable(QString::number(openedCounter)));
        }
    }
    else
    {
        qDebug() << "getExternalSoftwareOpenedCounter error: " << query.lastError().text();
    }
    return openedCounter;
}


QList<int> DbManager::getExternalSoftwareGamesWhitelist(QString externalSoftwareName)
{
    QList<int> gamesWhitelist;
    QString result;
    QSqlQuery query;
    query.prepare("SELECT games_whitelist FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while(query.next())
        {
            result = query.value(0).toString();
            foreach(QString gameId,result.split('|'))
            {
                gamesWhitelist.append(gameId.toInt());
            }
        }
    }
    else
    {
        qDebug() << "getExternalSoftwareGamesWhitelist error : " << query.lastError().text();
    }
    return gamesWhitelist;
}


QList<int> DbManager::getExternalSoftwareGamesOffList(QString externalSoftwareName)
{
    QList<int> gamesOffList;
    QString result;
    QSqlQuery query;
    query.prepare("SELECT games_off_list FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while(query.next())
        {
            result = query.value(0).toString();
            foreach(QString gameId,result.split('|'))
            {
                gamesOffList.append(gameId.toInt());
            }
        }
    }
    else
    {
        qDebug() << "getExternalSoftwareGamesOffList error : " << query.lastError().text();
    }
    return gamesOffList;
}


QStringList DbManager::getExternalSoftwarePlatformsOnList(QString externalSoftwareName)
{
    QStringList platformsOnList;
    QSqlQuery query;
    query.prepare("SELECT platforms_on_list FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while(query.next())
        {
            platformsOnList = query.value(0).toString().split('|');
        }
    }
    else
    {
        qDebug() << "getExternalSoftwarePlatormsOnList error : " << query.lastError().text();
    }
    return platformsOnList;
}

bool DbManager::getExternalSoftwareGameCanRun(QString extSoftName, int gameId){
    bool gameswhitelisted = getExternalSoftwareGamesWhitelist(extSoftName).contains(gameId);
    bool gamesoff = getExternalSoftwareGamesOffList(extSoftName).contains(gameId);
    bool platformon = getExternalSoftwarePlatformsOnList(extSoftName).contains(getGamePlatform(gameId));
    if (gameswhitelisted){
        return true;
    }
    else if(gamesoff){
            return false;
        }
    else if(platformon){
        return true;
    }
    return false;
}

bool DbManager::getExternalSoftwareMutliInstance(QString externalSoftwareName){
    bool state = false;
    QSqlQuery query;
    query.prepare("SELECT multi_instance FROM extsoft WHERE software_name=:software_name;");
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        while (query.next())
        {
            state = query.value(0).toBool();
        }
    }
    else
    {
        qDebug() << "getExternalSoftwareMultiInstance error: " << query.lastError().text();
    }
    return state;
}

bool DbManager::setExternalSoftwareGamesWhitelist(QString externalSoftwareName, QList<int> gamesWhitelist)
{
    QString gamesWhitelistDB;
    foreach(int gameId, gamesWhitelist)
    {
        gamesWhitelistDB += QString::number(gameId)+"|";
    }
    gamesWhitelistDB.chop(1);
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET games_whitelist=:games_whitelist WHERE software_name=:software_name;");
    query.bindValue(":games_whitelist",gamesWhitelistDB);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwareGamesWhitelist error: " << query.lastError().text();
    }
    return success;
}


bool DbManager::setExternalSoftwareGamesOffList(QString externalSoftwareName, QList<int> gamesOffList)
{
    QString gamesOffListDB;
    foreach(int gameId, gamesOffList)
    {
        gamesOffListDB += QString::number(gameId)+"|";
    }
    gamesOffListDB.chop(1);
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET games_off_list=:games_off_list WHERE software_name=:software_name;");
    query.bindValue(":games_off_list",gamesOffListDB);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwareGamesOffList error: " << query.lastError().text();
    }
    return success;
}

bool DbManager::setExternalSoftwarePlatformsOnList(QString externalSoftwareName, QStringList platformsOnList)
{
    QStringList list;
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET platforms_on_list=:platforms_on_list WHERE software_name=:software_name;");
    query.bindValue(":platforms_on_list",QString(platformsOnList.join('|')));
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwarePlatformsOnList error: " << query.lastError().text();
    }
    return success;
}


bool DbManager::setExternalSoftwarePath(QString externalSoftwareName, QString path)
//Provided an emulator_name and its path, this function sets it.
{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET path=:path WHERE software_name=:software_name;");
    query.bindValue(":path",path);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwarePath error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setExternalSoftwareSupportedBool(QString externalSoftwareName, bool supported)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET supported_extsoft=:supported_extsoft WHERE software_name=:software_name;");
    query.bindValue(":supported_extsoft",supported);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwareSupportedBool error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setExternalSoftwareOpenedCounter(QString externalSoftwareName, int openedCounter)
//Provided a game id and timePlayed, this function sets the time_played.

{
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET opened_counter=:opened_counter WHERE software_name=:software_name;");
    query.bindValue(":opened_counter",openedCounter);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwareOpenedCounter error:  " << query.lastError().text();
    }
    return success;
}

bool DbManager::setExternalSoftwareMutliInstance(QString externalSoftwareName, bool state){
    bool success = false;
    QSqlQuery query;
    query.prepare("UPDATE extsoft SET multi_instance=:multi_instance WHERE software_name=:software_name;");
    query.bindValue(":multi_instance",state);
    query.bindValue(":software_name",externalSoftwareName);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "setExternalSoftwareMultiInstance error:  " << query.lastError().text();
    }
    return success;
}
