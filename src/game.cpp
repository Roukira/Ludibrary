#include "mainwindow.h"
#include "game.h"
#include "ui_game.h"


Game::Game(QString gameName, QWidget *parent, int id, QString platformType,QString emulatorName, int steamid) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    DbManager db("data.db");
    m_gameTimePlayedOnSession = 0;
    m_gameTimePlayed = db.getGameTimePlayed(id);
    m_openedCounter = db.getGameOpenedCounter(id);
    qDebug(qUtf8Printable("time played at init :"+QString::number(m_gameTimePlayed)));
    if (m_gameTimePlayed>0)
    {
        showTimePlayed();
    }
    updateSettings(gameName,platformType,id);
    m_steamid = steamid;
    editGame *editWindow = new editGame(m_id,m_gameName,m_gamePlatform);
    m_editWindow = editWindow;
    QTimer *timer = new QTimer;
    m_timer = timer;
    m_timer->setInterval(m_timerInterval);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateGameTimePlayed()));
}

Game::Game(bool mode, QString gameName, QWidget *parent, int id, QString platformType, QString emulatorName, int steamid) : QWidget(parent), ui(new Ui::Game)
{
    ui->setupUi(this);
    if(mode)
    {
        ui->horizontalLayout->removeWidget(ui->labelInfo);
        ui->horizontalLayout->removeWidget(ui->buttonEdit);
        ui->horizontalLayout->removeWidget(ui->buttonFavorite);
        ui->horizontalLayout->removeWidget(ui->buttonPlay);

        ui->HoverInfo->layout()->addWidget(ui->labelInfo);
        ui->HoverButton->addWidget(ui->buttonPlay);
        ui->HoverButton->addWidget(ui->buttonEdit);
        ui->HoverButton->addWidget(ui->buttonFavorite);

        QFont f( "Arial", 11, QFont::Bold);
        ui->labelInfo->setFont(f);

        ui->horizontalLayout->deleteLater();

        this->setAttribute(Qt::WA_Hover, true);
    }
    DbManager db("data.db");
    ui->Hover->hide();
    qDebug(qUtf8Printable("debug10"));
    m_gameTimePlayedOnSession = 0;
    m_gameTimePlayed = db.getGameTimePlayed(id);
    m_openedCounter = db.getGameOpenedCounter(id);
    qDebug(qUtf8Printable("debug12"));
    m_state = db.getGameState(id);
    if(m_state.isEmpty())
    {
        m_state = "Never Played";
        db.setGameState(id,m_state);
    }
    qDebug(qUtf8Printable("debug13"));
    qDebug(qUtf8Printable(lastTimePlayedToString()));
    m_lastplayed = db.stringToDate(db.getGameLastPlayed(id));
    qDebug(qUtf8Printable("debug11"));
    qDebug(qUtf8Printable(lastTimePlayedToString()));
    if(m_lastplayed.isValid()&&(lastTimePlayedToString().contains("month")))
    {
        m_state = "Dropped";
        db.setGameState(id,m_state);
    }
    ui->HoverLast->setText(lastTimePlayedToString());
    ui->HoverState->setText(m_state);
    qDebug(qUtf8Printable("time played at init :"+QString::number(m_gameTimePlayed)));
    if (m_gameTimePlayed>0)
    {
        showTimePlayed();
    }
    updateSettings(gameName,platformType,id);
    m_steamid = steamid;
    editGame *editWindow = new editGame(m_id,m_gameName,m_gamePlatform);
    m_editWindow = editWindow;
    connect(m_editWindow,SIGNAL(updateParentState()),this,SLOT(updateMState()));
    QTimer *timer = new QTimer;
    m_timer = timer;
    m_timer->setInterval(m_timerInterval);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(updateGameTimePlayed()));
    QTimer *timer2 = new QTimer;
    m_lastplayedTimer = timer2;
    m_lastplayedTimer->setInterval(600*m_timerInterval);
    connect(m_lastplayedTimer,SIGNAL(timeout()),this,SLOT(updateLastTimePlayedLabel()));
    initializeBubbleButton();
}

Game::~Game()
{
    delete ui;
}

void Game::initializeBubbleButton(){
    DbManager db("data.db");
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("black"));
    effect->setOffset(1,1);
    QString info = db.getGameInfo(m_id);
    m_infoButton = new BubbleButton(32,32,info,this);
    ui->HoverButton->layout()->addWidget(m_infoButton);
}


void Game::updateGameInfo()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req( QUrl( QString("http://thegamesdb.net/api/GetGame.php?id="+QString::number(m_id))));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    QString text(""),gameTitle(""),platform(""),releaseDate(""),overview(""),genre(""),developer(""),publisher("");
    if (reply->error() == QNetworkReply::NoError)
    {
        //qDebug(qUtf8Printable(reply->readAll()));
        while (!(reply->atEnd()))
        {
            QString line = reply->readLine();
            if(line.contains("<GameTitle>"))
            {
                gameTitle = "Game Title : "+line;
                gameTitle = gameTitle.replace("<GameTitle>","");
                gameTitle = gameTitle.replace("</GameTitle>","");
            }
            else if (line.contains("<Platform>"))
            {
                platform = "Platform : "+line;
                platform = platform.replace("<Platform>","");
                platform = platform.replace("</Platform>","");
            }
            else if (line.contains("<ReleaseDate>"))
            {
                releaseDate = "Release Date : "+line;
                releaseDate = releaseDate.replace("<ReleaseDate>","");
                releaseDate = releaseDate.replace("</ReleaseDate>","");
            }
            else if (line.contains("<Overview>"))
            {
                qDebug(qUtf8Printable(line));
                overview += line;
                overview = overview.replace("<Overview>","");
                overview = overview.trimmed();
                overview += " ";
                while (!(overview.contains("</Overview>")))
                {
                    line = reply->readLine();
                    qDebug(qUtf8Printable(line));
                    overview += line;
                    //overview = overview.trimmed();
                }
                overview = overview.replace("</Overview>","");

            }
            else if (line.contains("<genre>"))
            {
                genre = "Genres : "+ line;
                genre = genre.replace("<genre>","");
                genre = genre.replace("</genre>","/");
                genre = genre.replace("<Genres>","");
                genre = genre.replace("</Genres>","|");
                genre = genre.split("|").at(0);
                genre = genre.left(genre.length()-1);
            }
            else if (line.contains("<Publisher>"))
            {
                publisher = "Publisher : "+line;
                publisher = publisher.replace("<Publisher>","");
                publisher = publisher.replace("</Publisher>","");
            }
            else if (line.contains("<Developer>"))
            {
                developer = "Developer : "+line;
                developer = developer.replace("<Developer>","");
                developer = developer.replace("</Developer>","");
            }
        }
        qDebug(qUtf8Printable(overview));
        qDebug(qUtf8Printable(genre));
        text = gameTitle+"\n"+platform+"\n"+releaseDate+"\n"+genre+"\n\n"+publisher+"\n"+developer+"\n\n"+overview;
        m_gameInfo = text;
        DbManager db("data.db");
        db.setGameInfo(m_id,text);
        m_infoButton->setInfoBubble(db.getGameInfo(m_id));
        delete reply;
    }
    else
    {
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
    }
}

void Game::updateSettings(QString gameName, QString gamePlatform, int id)
{
    m_gameName = gameName;
    m_gamePlatform = gamePlatform;
    m_id = id;
}


void Game::runGame()
{
    DbManager db("data.db");
    emit updateStatusBarProgressAsk("Initializing "+m_gameName+" startup.");
    emit updateStatusBarProgressBarAsk(10);
    qDebug(qUtf8Printable(m_gameName));
    qDebug(qUtf8Printable(m_gamePlatform));
    if (!db.getGamePath(m_id).isEmpty())
    {
        if(m_gamePlatform=="PC")
        {
            QString gamePath = db.getGamePath(m_id);
            QFile gamefile(gamePath);
            qDebug(qUtf8Printable(gamePath));
            bool steam = gamePath.contains(".url");
            if (steam||(gamefile.exists()))
            {
                if (steam)
                {
                    qDebug(qUtf8Printable("url"));
                    QUrl url = QUrl::fromLocalFile(gamePath);
                    bool ok = QDesktopServices::openUrl(url);
                    qDebug(qUtf8Printable("bool value : "));
                    qDebug(qUtf8Printable(QString::number(ok)));
                    if (ok)
                    {
                        qDebug(qUtf8Printable("steam id : "+QString::number(m_steamid)));
                        startGameTimer();
                    }
                }
                else
                {
                    qDebug(qUtf8Printable(gamePath));
                    if (gamePath.contains(".lnk"))
                    {
                        gamePath = QFile::symLinkTarget(gamePath);
                        qDebug(qUtf8Printable("symlink: "+gamePath));
                    }
                    QString gamePath2 = "\""+gamePath+"\"";
                    QProcess *exec = new QProcess(this);
                    QDir dir(gamePath);
                    dir.cdUp();
                    QDir::setCurrent(dir.absolutePath());
                    exec->setProgram(gamePath2);
                    if(exec->startDetached(&m_pid))
                    {
                        qDebug(qUtf8Printable(QString::number(m_pid)));
                        startGameTimer();
                    }
                }

            }
            else
            {
                QMessageBox::information(this, "Game launcher not provided", "You must provide game launcher to launch it. \nPlease click on the game's edit button.");
            }
        }
        else if(m_gamePlatform=="Nintendo Wii U")
        {
            QString emu_name = db.getPlatformEmulator(m_gamePlatform);
            qDebug(qUtf8Printable(emu_name));
            QString emu_path = db.getEmulatorPath(emu_name);
            qDebug(qUtf8Printable(emu_path));
            QString game_path = db.getGamePath(m_id);
            //QString command = "\""+emu_path +"\" -g \""+game_path+"\"";
            QString command = "\""+emu_path+ "\" -g \""+game_path+"\"";
            QStringList list;
            list.append("-g");
            list.append(game_path);
            qDebug(qUtf8Printable(command));
            QProcess *exec = new QProcess(this);
            if(exec->startDetached(emu_path,list,QString(),&m_pid))
            {
                qDebug(qUtf8Printable(QString::number(m_pid)));
                startGameTimer();
            }
        }
        else if(m_gamePlatform=="Nintendo 3DS")
        {
            QString emu_name = db.getPlatformEmulator(m_gamePlatform);
            qDebug(qUtf8Printable(emu_name));
            QString emu_path = db.getEmulatorPath(emu_name);
            qDebug(qUtf8Printable(emu_path));
            QString game_path = db.getGamePath(m_id);
            QString command = "\""+emu_path +"\" \""+game_path+"\"";
            qDebug() << command;
            QProcess *exec = new QProcess(this);
            exec->setProgram(command);
            if(exec->startDetached(&m_pid))
            {
                qDebug(qUtf8Printable(QString::number(m_pid)));
                startGameTimer();
            }
        }
        else if(m_gamePlatform=="Sony Playstation")
        {
            QString emu_name = db.getPlatformEmulator(m_gamePlatform);
            qDebug(qUtf8Printable(emu_name));
            QString emu_path = db.getEmulatorPath(emu_name);
            qDebug(qUtf8Printable(emu_path));
            QString game_path = db.getGamePath(m_id);
            QString command = "\""+emu_path +"\" -loadbin \""+game_path+"\" -nogui";
            QProcess *exec = new QProcess(this);
            exec->setProgram(command);
            if(exec->startDetached(&m_pid))
            {
                qDebug(qUtf8Printable(QString::number(m_pid)));
                startGameTimer();
            }
        }
        else
        {
            //QMessageBox::information(this, "Game launcher not supported", "Non PC games not supported yet. Sorry !");
            QString emu_name = db.getPlatformEmulator(m_gamePlatform);
            qDebug(qUtf8Printable(emu_name));
            QString emu_path = db.getEmulatorPath(emu_name);
            qDebug(qUtf8Printable(emu_path));
            QString game_path = db.getGamePath(m_id);
            QString command = emu_path + " \""+game_path+"\"";
            QProcess *exec = new QProcess(this);
            exec->setProgram(command);
            if(exec->startDetached(&m_pid))
            {
                qDebug(qUtf8Printable(QString::number(m_pid)));
                startGameTimer();
            }
        }
        emit updateStatusBarProgressAsk("");
        emit updateStatusBarProgressBarAsk(100);

    }
    else
    {
        QMessageBox::information(this, "Game launcher not provided", "You must provide game launcher to launch it. \nPlease click on the game's edit button.");
    }
}


void Game::startGameTimer()
{
    DbManager db("data.db");
    emit updateStatusBarProgressAsk("Starting "+m_gameName+".");
    emit updateStatusBarProgressBarAsk(50);
    MainWindow *mainwindow = m_editWindow->getMainWindow();
    db.updateGameLastPlayed(m_id);
    m_lastplayed = db.stringToDate(db.getGameLastPlayed(m_id));
    ui->HoverLast->setText(lastTimePlayedToString());
    ui->HoverState->setText("Playing");
    foreach (QString extsoft, db.getAllExternalSoftwares()){
        if (db.getExternalSoftwareGameCanRun(extsoft,m_id)){
            ExternalSoftware* extsoftPointer = mainwindow->getExternalSoftware(extsoft);
            if (!(extsoftPointer->isRunning()) || extsoftPointer->getMultiInstance()){
                QProcess *exec = new QProcess(this);
                qint64* extsoftPID = new qint64;
                exec->setProgram(db.getExternalSoftwarePath(extsoft));
                if(exec->startDetached(extsoftPID)){
                    db.setExternalSoftwareOpenedCounter(extsoft,db.getExternalSoftwareOpenedCounter(extsoft)+1);
                    extsoftPointer->setPid(*extsoftPID);
                    //store pid
                }
                else{
                    delete extsoftPID;
                }
            }

        }
    }
    incrementOpenedCounter();
    qDebug(qUtf8Printable("should notif..."));
    emit newNotificationAsk(m_id,"Playing now "+db.getGameTitle(m_id)+".");
    m_timer->start();
}

bool Game::IsProcessRunning(DWORD pid)
{
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

void Game::updateGameTimePlayed()
{
    DbManager db("data.db");
    if(m_gameTimePlayedOnSession%(30*(m_timerInterval/1000)))
    {
        db.updateGameLastPlayed(m_id);
        m_lastplayed = db.stringToDate(db.getGameLastPlayed(m_id));
        qDebug(qUtf8Printable("updateGameTimePlayed"));
        //qDebug(qUtf8Printable(lastTimePlayedToString()));
        //ui->HoverLast->setText(lastTimePlayedToString());
    }
    int steam_running_id;
    bool steam_running = false;
    m_gameTimePlayed += m_timerInterval/1000;
    m_gameTimePlayedOnSession += m_timerInterval/1000;
    bool time_set_worked = db.setGameTimePlayed(m_id,m_gameTimePlayed);
    if(time_set_worked)
    {
        qDebug(qUtf8Printable("haaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
    }
    showTimePlayed();
    qDebug(qUtf8Printable(QString::number(m_gameTimePlayed)));
    if (m_steamid>0)
    {
        if (m_gameTimePlayedOnSession<5)
        {
            return;
        }
        QSettings steam("HKEY_CURRENT_USER\\Software\\Valve\\Steam",QSettings::NativeFormat);
        qDebug(qUtf8Printable("steambool : "+QString::number(steam_running)));
        steam_running_id = steam.value("RunningAppID").toInt();
        qDebug(qUtf8Printable("steam_id_running : "+QString::number(steam_running_id)));
        steam_running = steam_running_id==m_steamid;
        qDebug(qUtf8Printable("steambool : "+QString::number(steam_running)));
    }
    if(!IsProcessRunning((DWORD)m_pid))
    {
        if(!steam_running)
        {
            if ((m_gamePlatform=="PC")&&(m_steamid==-1))
            {
                HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
                PROCESSENTRY32 pe;
                pe.dwSize = sizeof(PROCESSENTRY32);
                DWORD childPid = SearchChild(m_pid,h,pe);
                if (childPid!=m_pid)
                {
                    m_pid = childPid;
                    CloseHandle(h);
                    return;
                }
                CloseHandle(h);
            }
            m_timer->stop();
            m_gameTimePlayedOnSession=0;
            ui->HoverLast->setText(lastTimePlayedToString());
            if ((m_state == "Never Played")||(m_state == "Plan to Play")||(m_state == "Dropped"))
            {
                m_state = "Regularly Playing";
                db.setGameState(m_id,m_state);
            }
            ui->HoverState->setText(m_state);
            emit newNotificationAsk(m_id,db.getGameTitle(m_id)+" has been closed.");
        }

    }
}
qint64 Game::SearchChild(DWORD pid,HANDLE h, PROCESSENTRY32 pe)
{
    DWORD new_pid;
    QString exePath;
    if( Process32First(h, &pe)) {
        do {
                qDebug(qUtf8Printable(QString::number((int)pid)+" vs "+QString::number((int)pe.th32ParentProcessID)));

                if (pe.th32ParentProcessID == pid)
                {
                    qDebug(qUtf8Printable(QString::number((int)pid)+"Child process detected.\nShifting process id to child process id."));
                    new_pid = pe.th32ProcessID;
                    return new_pid;
                }
                else
                {
                    exePath = QString::fromWCharArray(pe.szExeFile);
                    qDebug(qUtf8Printable("exe path : "+exePath));
                    if(m_editWindow->getExeList().contains(exePath))
                    {
                        new_pid = pe.th32ProcessID;
                        return new_pid;
                    }
                    //works, add to take parent folder with qdir and compare with exepath from database.
                }
            } while( Process32Next(h, &pe));
    }
    return pid;
}

void Game::incrementOpenedCounter(){
    DbManager db("data.db");
    m_openedCounter++;
    db.setGameOpenedCounter(m_id,m_openedCounter);
    db.setPlatformOpenedCounter(m_gamePlatform,db.getPlatformOpenedCounter(m_gamePlatform)+1);
    if(m_gamePlatform!="PC")
    {
        QString emu_name = db.getPlatformEmulator(m_gamePlatform);
        db.setEmulatorOpenedCounter(emu_name,db.getEmulatorOpenedCounter(emu_name)+1);
    }
}
void Game::changeSettings()
{

}

void Game::updateMState()
{
    DbManager db("data.db");
    m_state = db.getGameState(m_id);
    ui->HoverState->setText(m_state);
}

QString Game::getGameName()
{
    return m_gameName;
}

QString Game::getPlatform()
{
    return m_gamePlatform;
}

int Game::getId()
{
    return m_id;
}

QString Game::getImagePath()
{
    return m_gameIconPath;
}

void Game::on_buttonPlay_pressed()
{
    this->runGame();
}

void Game::on_buttonEdit_pressed()
{
    m_editWindow->show();
}


int Game::getTimePlayed()
{
    return m_gameTimePlayed;
}

void Game::showTimePlayed()
{
    int play_time = getTimePlayed();
    int timePlayedCalculated = m_gameTimePlayed;
    QString time_played;
    if (play_time == 0)
    {
        ui->labelInfo->setText("Not played yet");
    }
    else if (play_time <60)
    {
        time_played = QString::number(timePlayedCalculated);
        if (timePlayedCalculated==1) ui->labelInfo->setText(time_played+" second played ");
        else ui->labelInfo->setText(time_played+" seconds played ");
    }
    else if (play_time < 3600)
    {
        timePlayedCalculated /=60;
        time_played = QString::number(timePlayedCalculated);
        if (timePlayedCalculated==1) ui->labelInfo->setText(time_played+" minute played ");
        else ui->labelInfo->setText(time_played+" minutes played ");
    }
    else
    {
        timePlayedCalculated /= 3600;
        time_played = QString::number(timePlayedCalculated);
        if (timePlayedCalculated == 1) ui->labelInfo->setText(time_played+" hour played");
        else ui->labelInfo->setText(time_played+" hours played");
    }


}

void Game::gameQuit(int exitStatus)
{
    DbManager db("data.db");
    qDebug(qUtf8Printable("app quit :)"));
    m_timer->stop();
    emit newNotificationAsk(m_id,"Stopped playing "+db.getGameTitle(m_id)+".");
}


void Game::getImageFromId(int id)
{
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://thegamesdb.net/api/GetArt.php?id="+QString::number(id)));
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(showSearchReply(QNetworkReply*)));
    mgr->get(req);
}

void Game::showSearchReply(QNetworkReply* reply)
{
    QString url("");
    qDebug(qUtf8Printable("nooooo"));
    if (reply->error() == QNetworkReply::NoError)
    {
        while(!(reply->atEnd()))
        {
            QString line = reply->readLine();
            qDebug(qUtf8Printable(line));
            if (line.contains("<boxart side=\"front\""))
            {
                url = line.section('>',1,2);
                url = url.section('<',0,0);
                //url = url.trimmed();
                url = "http://thegamesdb.net/banners/"+url;
                QUrl imageUrl(url);
                FileDownloader* m_pImgCtrl = new FileDownloader(imageUrl,m_id,this);
                connect(m_pImgCtrl,SIGNAL(downloaded()),this,SLOT(setupGameWidget()));
            }
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
        return;
    }
}

void Game::setupGameWidget()
{
    DbManager db("data.db");
    ui->labelGameName->setText(db.getGameTitle(m_id));
    ui->labelPlatform->setText(db.getGamePlatform(m_id));
    QPixmap image = QPixmap();
    image.loadFromData(db.getGameImage(m_id));
    ui->labelGameImage->setPixmap(image);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("black"));
    effect->setOffset(1,1);
    ui->labelGameName->setGraphicsEffect(effect);
    QGraphicsDropShadowEffect *effect2 = new QGraphicsDropShadowEffect(this);
    effect2->setBlurRadius(0);
    effect2->setColor(QColor("black"));
    effect2->setOffset(1,1);
    ui->labelPlatform->setGraphicsEffect(effect2);
    QGraphicsDropShadowEffect *effect3 = new QGraphicsDropShadowEffect(this);
    effect3->setBlurRadius(0);
    effect3->setColor(QColor("black"));
    effect3->setOffset(1,1);
    ui->labelInfo->setGraphicsEffect(effect3);
    /*
    QGraphicsDropShadowEffect *effect4 = new QGraphicsDropShadowEffect(this);
    effect3->setBlurRadius(0);
    effect3->setColor(QColor("black"));
    effect3->setOffset(1,1);
    ui->HoverState->setGraphicsEffect(effect4);
    QGraphicsDropShadowEffect *effect5 = new QGraphicsDropShadowEffect(this);
    effect3->setBlurRadius(0);
    effect3->setColor(QColor("black"));
    effect3->setOffset(1,1);
    ui->HoverLast->setGraphicsEffect(effect5);*/
    labelSize();
    emit newNotificationAsk(m_id,db.getGameTitle(m_id)+" has successfully been added to the game list.");

}

void Game::labelSize()
{
    if (ui->labelGameName->text().length()<20)
    {
        QFont f( "Arial", 18, QFont::Bold);
        ui->labelGameName->setFont(f);
    }
    else if (ui->labelGameName->text().length()<50)
    {
        QFont f( "Arial", 14, QFont::Bold);
        ui->labelGameName->setFont(f);
    }
    else if(ui->labelGameName->text().length()<75)
    {
        QFont f( "Arial", 11, QFont::Bold);
        ui->labelGameName->setFont(f);
    }
    else
    {
        QFont f( "Arial", 10, QFont::Bold);
        ui->labelGameName->setFont(f);
    }

    if (ui->labelPlatform->text().length()<25)
    {
        QFont f( "Arial", 14, QFont::Bold);
        ui->labelPlatform->setFont(f);
    }
    else if (ui->labelPlatform->text().length()<30)
    {
        QFont f( "Arial", 12, QFont::Bold);
        ui->labelPlatform->setFont(f);
    }
    else if(ui->labelPlatform->text().length()<35)
    {
        QFont f( "Arial", 11, QFont::Bold);
        ui->labelPlatform->setFont(f);
    }
    else
    {
        QFont f( "Arial", 10, QFont::Bold);
        ui->labelPlatform->setFont(f);
    }
}

void Game::addExternalSoftwareWidgetAsk(QString extSoftName){
    m_editWindow->addExternalSoftwareWidget(extSoftName);
}

bool Game::isExternalSoftwareLoaded(QString extSoftName){
    return m_editWindow->IsExternalSoftwareLoaded(extSoftName);
}

void Game::hoverEnter(QHoverEvent * event)
{
    qDebug() << Q_FUNC_INFO << this->objectName();
    ui->Hover->show();


}
void Game::hoverLeave(QHoverEvent * event)
{
    qDebug() << Q_FUNC_INFO << this->objectName();
    ui->Hover->hide();
}

bool Game::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(e));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

QString Game::lastTimePlayedToString()
{
    if (!(m_lastplayed.isValid()))
    {
        qDebug(qUtf8Printable("hello never"));
        return "Last time played : Never";
    }
    QString result("Never");
    QString time("");
    int secs = m_lastplayed.secsTo(QDateTime::currentDateTime());
    qDebug(qUtf8Printable("secs : "+QString::number(secs)));
    if (secs == 0)
    {
        result = "Last time played : Now";
    }
    else if (secs <60)
    {
        time = QString::number(secs);
        if (secs==1) result = "Last time played : "+time+" second ago.";
        else result = result = "Last time played : "+time+" seconds ago.";

    }
    else if (secs < 3600)
    {
        secs /= 60;
        time = QString::number(secs);
        if (secs == 1) result = "Last time played : "+time+" minute ago.";
        else result = "Last time played : "+time+" minutes ago.";

    }
    else if(secs < 86400)
    {
        secs/= 3600;
        time = QString::number(secs);
        if (secs == 1) result = "Last time played : "+time+" hour ago.";
        else result = "Last time played : "+time+" hours ago.";

    }
    else if(secs < 2678400)
    {
        secs /=86400;
        time = QString::number(secs);
        if (secs==1) result = "Last time played : "+time+" day ago.";
        else result = "Last time played : "+time+" days ago.";

    }
    else
    {
        secs /= 2678400;
        time = QString::number(secs);
        if (secs == 1) result = "Last time played : "+time+" month ago.";
        else result = "Last time played : "+time+" months ago.";
    }
    qDebug(qUtf8Printable(result));
    return result;
}

void Game::updateLastTimePlayedLabel()
{
    ui->HoverLast->setText(lastTimePlayedToString());
}
