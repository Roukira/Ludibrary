#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    installEventFilter(this);
    //start->setText("Initializing application...");
}

void MainWindow::startApp(StartUpWidget *start){
    setStartUp();
    start->setText("Loading platforms...");
    loadAllTags();
    start->setText("Loading games...");
    loadAllGames();
    start->setText("Resizing application...");
    resizeWindow();
    setSysTray();
    start->setText("Loading emulators...");
    loadAllEmulators();
    start->setText("Loading statistics...");
    m_statsmonitor->updateStatistics();
    start->setText("Loading external softwares...");
    loadAllExternalSoftwares();
    start->setText("Launching application...");
    m_activeBubble = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setStartUp()

/*This function sets start up window management
*/

{
    this->setWindowFlags(Qt::FramelessWindowHint);

    Settings *settings = new Settings;
    m_settings = settings;
    connect(m_settings,SIGNAL(smoothScrolling(bool)),this,SLOT(updateSmoothScrolling(bool)));
    m_settings->loadSettings();

    ListedPCGameDialog *listDialog = new ListedPCGameDialog;
    m_listDialog = listDialog;

    ListEmulatorsDialog *listEmulator = new ListEmulatorsDialog;
    m_listEmulatorsDialog = listEmulator;
    connect(m_listEmulatorsDialog,SIGNAL(newEmulator()),this,SLOT(loadAllEmulators()));

    AddExternalSoftwareDialog *extSoftDialog = new AddExternalSoftwareDialog;
    m_add_extsoft_dialog = extSoftDialog;
    connect(m_add_extsoft_dialog,SIGNAL(acceptedSoftware(QString,QString,QStringList)),this,SLOT(addExternalSoftware(QString,QString,QStringList)));

    Help *help = new Help;
    m_help = help;

    if(!QFile::exists("settings.ini"))
    {
        help->show();
        initSupportedEmulators();
    }

    QNetworkAccessManager *mgr = new QNetworkAccessManager;
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(showSearchReply(QNetworkReply*)));
    m_mgrSearch = mgr;

    QNetworkRequest req(QUrl(QString("")));
    m_replySearch = mgr->get(req);

    QMovie *movie = new QMovie(":/Images/gifSearching");
    ui->labelGif->setMovie(movie);
    movie->start();
    ui->labelGif->hide();
    ui->NoResults->hide();

    NotificationQueue *notif = new NotificationQueue;
    m_notificationQueue = notif;
    connect(this,SIGNAL(newNotification(Notification*)),m_notificationQueue,SLOT(queueNotification(Notification*)));

    StatsMonitor *statsmonitor = new StatsMonitor;
    m_statsmonitor = statsmonitor;
    connect(m_statsmonitor,SIGNAL(updateGeneralQuery(QString,QString,QString,QString,QString)),this,SLOT(updateGeneralReply(QString,QString,QString,QString,QString)));
    connect(m_statsmonitor,SIGNAL(createPlatformQuery(QLabel*,QLabel*,QLabel*,QLabel*)),this,SLOT(createPlatformReply(QLabel*,QLabel*,QLabel*,QLabel*)));
    connect(m_statsmonitor,SIGNAL(createEmulatorQuery(QLabel*,QLabel*,QLabel*,QLabel*)),this,SLOT(createEmulatorReply(QLabel*,QLabel*,QLabel*,QLabel*)));
    connect(m_statsmonitor,SIGNAL(createExtSoftQuery(QLabel*,QLabel*)),this,SLOT(createExtSoftReply(QLabel*,QLabel*)));

    QLabel *statusBarState = new QLabel;
    QLabel *statusBarProgress = new QLabel;
    QProgressBar *statusBarProgressBar = new QProgressBar;
    QFont fontLabel("Arial",9);
    fontLabel.setBold(true);
    fontLabel.setKerning(true);

    statusBarState->setFont(fontLabel);
    statusBarProgress->setFont(fontLabel);

    //statusBarProgressBar->setValue(50);
    m_statusBarState = statusBarState;
    m_statusBarProgressLabel = statusBarProgress;
    m_statusBarProgressBar = statusBarProgressBar;


    ui->statusBar->addPermanentWidget(m_statusBarState,8);
    ui->statusBar->addPermanentWidget(m_statusBarProgressLabel,1);
    ui->statusBar->addPermanentWidget(m_statusBarProgressBar,1);

    m_statusBarProgressLabel->hide();
    m_statusBarProgressBar->hide();


}

void MainWindow::setSysTray()

/*This function sets up the mainwindow system tray.
*/

{
    QSystemTrayIcon *appTray = new QSystemTrayIcon;
    appTray->setIcon(QIcon(":/Images/appIcon"));
    QMenu *traymenu = new QMenu;
    QAction *quitApp = new QAction("Quit Ludibrary",this);
    traymenu->addAction(quitApp);
    connect(quitApp,SIGNAL(triggered()),this,SLOT(closeApp()));
    appTray->setContextMenu(traymenu);
    appTray->show();
    connect(appTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(slotActivated(QSystemTrayIcon::ActivationReason)));
    m_systemTray = appTray;
}


void MainWindow::on_buttonAdd_clicked()
{
    int choice = AddDialog().getChoice();
    if(choice==1)
    {
        searchAllGames();
    }
    else if(choice==2)
    {
        //create a add Dialog for this asking for gameId(int id = -game_counter), name, path, platform, image : copy emulatordialog ui.
        //addgamemanually, later
    }
    else if(choice==3)
    {
        addEmulator();
    }
    else if(choice ==4)
    {
        m_add_extsoft_dialog->showUp();

    }


}

void MainWindow::updateStatusBarState(QString state){
    if (state==""){
        m_statusBarState->setText("");
    }
    else{
        m_statusBarState->setText("\t"+state);
    }

}

void MainWindow::updateStatusBarProgress(QString progress){
    if (progress==""){
        m_statusBarProgressLabel->setText("");
    }
    else{
        m_statusBarProgressLabel->setText(progress);
    }
}

void MainWindow::updateStatusBarProgressBar(int value){
    if (value <=0){
        m_statusBarProgressBar->setValue(0);
        m_statusBarProgressBar->hide();
        return;
    }
    else if(value >=100){
        m_statusBarProgressBar->setValue(100);
        m_statusBarProgressBar->hide();
        return;
    }
    if (m_statusBarProgressBar->isHidden()){
        m_statusBarProgressBar->show();
    }
    m_statusBarProgressBar->setValue(value);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_systemTray->deleteLater();
}

void MainWindow::addExternalSoftware(QString name, QString path, QStringList platforms)
{
    ExternalSoftware *extSoft = new ExternalSoftware(name,path,platforms);
    m_extsoftList.append(extSoft);
    connect(this,SIGNAL(updateExternalSoftwareExceptions()),extSoft,SLOT(updateExceptions()));
    ui->ExtSoftScrolling->layout()->addWidget(extSoft->getWidget());
    foreach(Game* gameWidget,m_gameWidgets){
        if(!(gameWidget->isExternalSoftwareLoaded(name))){
            gameWidget->addExternalSoftwareWidgetAsk(name);
        }
    }
}
void MainWindow::loadGame(Game* gameWidget)
{
    DbManager db("data.db");
    qDebug(qUtf8Printable("debug2"));
    QListWidgetItem *game = new QListWidgetItem;
    connect(gameWidget,SIGNAL(newNotificationAsk(int,QString)),this,SLOT(notificationAsked(int,QString)),Qt::UniqueConnection);
    connect(gameWidget,SIGNAL(updateStatusBarStateAsk(QString)),this,SLOT(updateStatusBarState(QString)));
    connect(gameWidget,SIGNAL(updateStatusBarProgressAsk(QString)),this,SLOT(updateStatusBarProgress(QString)));
    connect(gameWidget,SIGNAL(updateStatusBarProgressBarAsk(int)),this,SLOT(updateStatusBarProgressBar(int)));
    QVariant v;
    v.setValue(gameWidget->getId());
    game->setData(Qt::UserRole, v);
    game->setSizeHint(gameWidget->size());
    ui->listWidgetAllGames->addItem(game);
    ui->listWidgetAllGames->setItemWidget(game,gameWidget);
    qDebug(qUtf8Printable("debug1"));
    foreach(QString extSoftName,db.getAllExternalSoftwares()){
        gameWidget->addExternalSoftwareWidgetAsk(extSoftName);
    }
    m_gameWidgets.append(gameWidget);
}

void MainWindow::loadAllGames()
{
    DbManager db("data.db");
    QList<int> allGames = db.getAllGames();
    QString gameTitle,platform;
    int steamid;
    foreach(int gameId,allGames)
    {
        gameTitle = db.getGameTitle(gameId);
        platform = db.getGamePlatform(gameId);
        steamid = db.getGameSteamId(gameTitle,platform);
        Game *gameWidget = new Game(m_settings->getNewGameDesign(),gameTitle,this,gameId,platform,"None",steamid);
        gameWidget->setupGameWidget();
        loadGame(gameWidget);
    }
}

void MainWindow::loadAllExternalSoftwares(){

    DbManager db("data.db");
    foreach (QString extsoft, db.getAllExternalSoftwares()){
        qDebug(qUtf8Printable("loading : "+extsoft));
        addExternalSoftware(extsoft,db.getExternalSoftwarePath(extsoft),db.getExternalSoftwarePlatformsOnList(extsoft));
    }
}

ExternalSoftware* MainWindow::getExternalSoftware(QString extSoftName){
    foreach(ExternalSoftware* extsoft, m_extsoftList){
        if (extsoft->getName()==extSoftName) return extsoft;
    }
    return NULL;
}

void MainWindow::addGame(QString gameName, QString gamePlatform, int id, int steamid)
{
    qDebug(qUtf8Printable("debug4"));
    Game *gameWidget = new Game(m_settings->getNewGameDesign(),gameName,this,id,gamePlatform,"None",steamid);
    qDebug(qUtf8Printable("debug5"));
    DbManager db("data.db");
    if ((gameName.isEmpty())||(gamePlatform.isEmpty()))
    {
        QMessageBox::information(this, "Game not valid", "The game you provided is not valid, empty name or platform won't work !");
        gameWidget->deleteLater();
        return;
    }
    else if ((!db.checkAddGamePossible(id)))
    {
        QMessageBox::information(this, "Game already added", "The game you selected has already been added !");
        gameWidget->deleteLater();
        return;
    }
    else
    {
        db.addGameEntry(id,gameName,gamePlatform,steamid);
        connect(gameWidget,SIGNAL(newNotificationAsk(int,QString)),this,SLOT(notificationAsked(int,QString)),Qt::UniqueConnection);
        gameWidget->getImageFromId(id);
        gameWidget->updateGameInfo();
        addTag(gamePlatform);
    }
    qDebug(qUtf8Printable("debug3"));
    loadGame(gameWidget);
    m_statsmonitor->updateStatistics();
}


void MainWindow::notificationAsked(int gameId, QString text)
{
    if (gameId>=0)
    {
        DbManager db("data.db");
        Notification* notif = new Notification(text,gameId,this);
        emit newNotification(notif);
    }
}

void MainWindow::updateGeneralReply(QString games, QString platform, QString emulator, QString playtime, QString extsoft)
{
    ui->GameCounter->setText(games);
    ui->PlatformCounter->setText(platform);
    ui->EmulatorCounter->setText(emulator);
    ui->ExtSoftCounter->setText(extsoft);
    ui->TimeSpent->setText(playtime);
}

void MainWindow::createPlatformReply(QLabel *platformName, QLabel *platformGameCounter, QLabel *platformTimePlayed, QLabel *platformOpenedCounter)
{
    ui->PlatformStats->layout()->addWidget(platformName);
    ui->PlatformStats->layout()->addWidget(platformGameCounter);
    ui->PlatformStats->layout()->addWidget(platformOpenedCounter);
    ui->PlatformStats->layout()->addWidget(platformTimePlayed);
}

void MainWindow::createEmulatorReply(QLabel *emulatorName, QLabel *emulatorGameCounter, QLabel *emulatorTimePlayed, QLabel *emulatorOpenedCounter)
{
    ui->EmulatorStats->layout()->addWidget(emulatorName);
    ui->EmulatorStats->layout()->addWidget(emulatorGameCounter);
    ui->EmulatorStats->layout()->addWidget(emulatorOpenedCounter);
    ui->EmulatorStats->layout()->addWidget(emulatorTimePlayed);
}

void MainWindow::createExtSoftReply(QLabel *extSoftName, QLabel *extSoftOpenedCounter){
    ui->ExtSoftStats->layout()->addWidget(extSoftName);
    ui->ExtSoftStats->layout()->addWidget(extSoftOpenedCounter);
}

void MainWindow::addTag(QString tagName)
{
    QString platform = tagName;
    DbManager db("data.db");
    if (db.addPlatformEntry(platform))
    {
        loadTag(platform);
        loadAllEmulators();
        Notification* notif = new Notification(platform + " has succesfully been added to the platform list.",-1,this);
        emit newNotification(notif);
    }
    db.incrementPlatformGameCounter(platform,true);

}

void MainWindow::loadTag(QString platform)
{
    QListWidgetItem *platformTag = new QListWidgetItem;
    QVariant v;
    v.setValue(platform);
    platformTag->setData(Qt::UserRole, v);
    QPushButton *tagWidget = new QPushButton(platform);
    tagWidget->setStyleSheet("QPushButton {background-color: transparent;\ncolor: white;}\n\nQPushButton:checked{background-color: rgba(170, 255, 255, 30);}");
    tagWidget->setFont(QFont("Arial",11));
    tagWidget->setCheckable(true);
    int width = tagWidget->fontMetrics().width(tagWidget->text());
    platformTag->setSizeHint(QSize(width,30));
    ui->listWidgetAllTags->addItem(platformTag);
    ui->listWidgetAllTags->setItemWidget(platformTag,tagWidget);
    /*if (platform!="PC")
    {
        loadAllEmulators();
    }*/
}

void MainWindow::loadAllTags()
{
    DbManager db("data.db");
    QStringList allPlatforms = db.getAllPlatforms();
    foreach(QString platform, allPlatforms)
    {
        loadTag(platform);
    }

}


void MainWindow::addEmulator()
{
    m_listEmulatorsDialog->clearContent();
    m_listEmulatorsDialog->show();
}

void MainWindow::loadAllEmulators()
{
    DbManager db("data.db");
    QStringList emulators = db.getAllEmulators();
    QStringList platforms = db.getAllPlatforms();
    QStringList new_platforms;
    QString path;
    ui->listWidgetEmulators->clear();
    foreach(QString emulatorName, emulators)
    {
        path = db.getEmulatorPath(emulatorName);
        new_platforms.clear();
        foreach (QString platform, platforms)
        {
            if(db.getPlatformEmulator(platform).trimmed()==emulatorName.trimmed())
            {
                new_platforms.append(platform);
            }
        }
        Emulator *emulator = new Emulator(path, emulatorName, new_platforms);
        QListWidgetItem *emulatorWidget = new QListWidgetItem;
        QVariant v;
        v.setValue(emulatorName);
        emulatorWidget->setData(Qt::UserRole, v);
        emulatorWidget->setSizeHint(emulator->size());
        ui->listWidgetEmulators->addItem(emulatorWidget);
        ui->listWidgetEmulators->setItemWidget(emulatorWidget,emulator);

    }
}

void MainWindow::initSupportedEmulators()
{
    DbManager db("data.db");
    QDir dir(m_settings->getEmulatorsDir());
    dir.setFilter(QDir::AllDirs|QDir::NoDotAndDotDot);
    QStringList list = dir.entryList();
    //QStringList platforms;
    QString name;
    int index = list.count();
    for (int i=0;i<index;i++)
    {
        name = list.at(i);
        name = name.trimmed();
        db.addEmulatorEntry(name,m_settings->getEmulatorsDir()+"/"+name+"/"+name+".exe");
        name = "";
    }
}

void MainWindow::on_buttonDeleteSelection_clicked()
{
    DbManager db("data.db");
    QVariant v;
    int gameId;
    int itemNumber(ui->listWidgetAllGames->currentRow());
    if (itemNumber>-1)
    {
        v = ui->listWidgetAllGames->item(itemNumber)->data(Qt::UserRole);
        gameId = v.value<int>();
        QString platform = db.getGamePlatform(gameId);
        db.deleteGameEntry(gameId);
        db.incrementPlatformGameCounter(platform,false);
        if (m_settings->getDeletePlatform())
        {
            qDebug(qUtf8Printable(QString::number(db.getPlatformGameCounter(platform))));
            if(db.getPlatformGameCounter(platform)==0)
            {
                db.deletePlatformEntry(platform);
                int index = ui->listWidgetAllTags->count();
                int counter = 0;
                QVariant v2;
                QString platformv2;
                bool ok =  true;
                while (ok&&(counter<index))
                {
                    v2 = ui->listWidgetAllTags->item(counter)->data(Qt::UserRole);
                    platformv2 = v2.value<QString>();
                    if (platform==platformv2)
                    {
                        qDebug(qUtf8Printable("ok3"));
                        delete ui->listWidgetAllTags->item(counter);
                        ok = false;
                    }
                    counter++;
                }
                if(platform !="PC")
                {
                    loadAllEmulators();
                }
                m_statsmonitor->deletePlatformStat(platform);
            }
        }
        //ask later for are you sure and dont ask me next time tick box
        delete ui->listWidgetAllGames->item(itemNumber);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
  m_Diff = event->pos();

  setCursor(QCursor(Qt::SizeAllCursor));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  QPoint p = event->globalPos();

  this->move(p - m_Diff);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event);

  setCursor(QCursor(Qt::ArrowCursor));
}


bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == this && event->type() == QEvent::Wheel) {
        qDebug() << "SCroll";
        return true;
    }
    return false;
}

void MainWindow::setActiveBubble(BubbleText *bubble)
{
    m_activeBubble = bubble;
}

BubbleText* MainWindow::getActiveBubble()
{
    return m_activeBubble;
}
void MainWindow::on_buttonExit_clicked()
{
    this->hide();
    m_settings->hide();
    m_help->hide();
}

void MainWindow::on_buttonHide_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
    m_settings->setWindowState(Qt::WindowMinimized);
    m_help->setWindowState(Qt::WindowMinimized);
}

void MainWindow::slotActivated(QSystemTrayIcon::ActivationReason value)
{
    if(value==3)
    {
        this->show();
        if (m_settings->getState())
        {
            m_settings->show();
        }
        if (m_help->getState())
        {
            m_help->show();
        }
    }
}

void MainWindow::closeApp()
{
    m_systemTray->setVisible(false);
    this->close();
    m_settings->close();
    m_help->close();
    m_listDialog->close();
    m_listEmulatorsDialog->close();
}

void MainWindow::on_buttonSettings_clicked()
{
    m_settings->show();
    m_settings->setState(true);

}
void MainWindow::on_buttonHelp_clicked()
{
    m_help->show();
    m_help->setState(true);

}

void MainWindow::on_lineEditSearch_textChanged(QString text)
{
   int index = ui->tabWidget->currentIndex();
   if (index==0)
   {
       QVariant v;
       QString gameTitle;
       int gameId;
       QString textlowered = text.toLower();
       int itemNumber(ui->listWidgetAllGames->count());
       for(int counter =0;counter<itemNumber;counter++)
       {
           v = ui->listWidgetAllGames->item(counter)->data(Qt::UserRole);
           gameId = v.value<int>();
           DbManager db("data.db");
           gameTitle = db.getGameTitle(gameId);
           if(gameTitle.toLower().contains(textlowered))
           {
               ui->listWidgetAllGames->item(counter)->setHidden(false);
           }
           else
           {
               ui->listWidgetAllGames->item(counter)->setHidden(true);
           }
       }
   }
   else if (index ==1)
   {
       ui->NoResults->hide();
       if (text!="")
       {
            qDebug(qUtf8Printable("foo"));
            if(m_replySearch->isRunning())
            {
                qDebug(qUtf8Printable("foo0"));
                m_replySearch->abort();
                m_replySearch->deleteLater();
            }

            ui->listWidgetSearchGames->clear();
            qDebug(qUtf8Printable("foo1"));
            QNetworkRequest req(QUrl(QString("http://thegamesdb.net/api/GetGamesList.php?name="+text)));
            qDebug(qUtf8Printable("foo2"));
            m_replySearch = m_mgrSearch->get(req);
            qDebug(qUtf8Printable("foo3"));
            ui->labelGif->show();


       }
       else
       {
           if(m_replySearch->isRunning())
           {
               m_replySearch->abort();
           }
           ui->listWidgetSearchGames->clear();
       }

   }
}

void MainWindow::showSearchReply(QNetworkReply* reply)
{
    ui->labelGif->hide();
    ui->listWidgetSearchGames->clear();
    int games(0);
    if (reply->error() == QNetworkReply::NoError)
    {
        QString id,GameTitle,Platform,ReleaseDate;
        while ((!(reply->atEnd())))
        {
            id="";
            GameTitle="";
            Platform="";
            ReleaseDate="";
            QString line = reply->readLine();
            if (line=="<Game>\n")
            {
                id = reply->readLine();
                id = id.remove(0,4);
                id = id.left(id.length()-6);
                GameTitle = reply->readLine();
                GameTitle = GameTitle.remove(0,11);
                GameTitle = GameTitle.left(GameTitle.length()-13);
                QString NextLine = reply->readLine();
                if (NextLine.left(13)=="<ReleaseDate>")
                {
                    ReleaseDate = NextLine.remove(0,13);
                    ReleaseDate = ReleaseDate.left(ReleaseDate.length()-15);
                    Platform = reply->readLine();
                    Platform = Platform.remove(0,10);
                    Platform = Platform.left(Platform.length()-12);
                }
                else
                {
                    Platform = NextLine.remove(0,10);
                    Platform = Platform.left(Platform.length()-12);
                }
                if (ReleaseDate=="")
                {
                    qDebug(qUtf8Printable(GameTitle+" "+id+" "+Platform));

                }
                else
                {
                    qDebug(qUtf8Printable(GameTitle+" "+id+" "+ReleaseDate+" "+Platform));
                }
                    QListWidgetItem *game = new QListWidgetItem;
                    SearchGame *gameWidget = new SearchGame(0,GameTitle,Platform,ReleaseDate,id.toInt());
                    connect(gameWidget,SIGNAL(askAddGame(SearchGame*)),this,SLOT(addGameSearched(SearchGame*)));
                    game->setSizeHint(gameWidget->size());
                    ui->listWidgetSearchGames->addItem(game);
                    ui->listWidgetSearchGames->setItemWidget(game,gameWidget);
                    games++;
                    if(games>=40)
                    {
                        break;
                    }


            }
       }
       if(games==0)
       {
            ui->NoResults->show();
       }

    }
    else
    {
        qDebug() << "Failure" <<reply->errorString();
    }
}

void MainWindow::on_comboBoxSort_currentIndexChanged(const QString& text)
{
    if (text=="Alphabetical")
    {
        ui->listWidgetAllGames->sortItems(Qt::AscendingOrder);
    }
    else if(text=="Release Date")
    {

    }
    else if(text=="Last Played")
    {

    }
}

void MainWindow::tagSelected(QListWidgetItem* item)
{

}


void MainWindow::resizeWindow()
{
    QSize screensize = qApp->screens()[0]->size();
    this->resize(screensize.width()*0.9,screensize.height()*0.9);
    m_notificationQueue->setCurrentMainWindowSize(this->size());
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   //ui->listWidgetAllGames->sortItems();
   qDebug(qUtf8Printable("hello24"));
}

QStringList MainWindow::searchAllGames()
{
    m_listDialog->show();
    m_listDialog->clearContent();
    connect(m_listDialog,SIGNAL(askAddGames(QList<QString>*)),this,SLOT(addPCAskedGames(QList<QString>*)));
    QStringList list;
    QSettings steam("HKEY_CURRENT_USER\\Software\\Valve\\Steam",QSettings::NativeFormat);
    QString SteamPath = steam.value("SteamPath").toString();
    QFile gameacf;
    QString gameacffile(""),gamename("");
    QString gamesteamid("");
    qDebug(qUtf8Printable(SteamPath));
    SteamPath+="/steamapps";
    QDirIterator it(SteamPath, QStringList() << "*.acf", QDir::Files);
    while (it.hasNext())
    {
        gamename = "";
        gamesteamid=-1;
        gameacffile = it.next();
        qDebug(qUtf8Printable(gameacffile));
        gameacf.setFileName(gameacffile);
        gameacf.open(QIODevice::ReadOnly);
        QTextStream gameinfo(&gameacf);
        gameinfo.setCodec("UTF-8");
        QString line;
        while (!gameinfo.atEnd())
        {
            line = gameinfo.readLine();
            if (line.contains("name"))
            {
                gamename = line.split("		").at(1);
                gamename = gamename.replace("™","");
                gamename = gamename.replace("\"","");
                gamename = gamename.trimmed();
            }
            else if(line.contains("appid"))
            {
                gamesteamid = line.split("		").at(1);
                gamesteamid = gamesteamid.replace("™","");
                gamesteamid = gamesteamid.replace("\"","");
                gamesteamid = gamesteamid.trimmed();
            }

        }
        addPCGame(gamename,gamesteamid.toInt(),m_listDialog);
        gameacf.close();

    }
    m_listDialog->setButtonEnabled();

    return list;
}

void MainWindow::addPCGame(QString gameName,int gameSteamid, ListedPCGameDialog* dialog)
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QNetworkRequest req(QUrl(QString("http://thegamesdb.net/api/GetGame.php?exactname="+gameName+"&platform=PC")));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    QString gameTitle(""),gameTitle2(""),id("");
    if (reply->error() == QNetworkReply::NoError)
    {
        while(!reply->atEnd())
        {
            QString line = reply->readLine();
            if(line.contains("<id>"))
            {
                id = line;
                id = id.replace("<id>","");
                id = id.replace("</id>","");
            }
            else if(line.contains("<GameTitle>"))
            {
                gameTitle = line;
                gameTitle = gameTitle.replace("<GameTitle>","");
                gameTitle = gameTitle.replace("</GameTitle>","");
                gameTitle2 = gameTitle.replace(":","");
                break;
            }
        }
        qDebug(qUtf8Printable(gameName.toLower()));
        qDebug(qUtf8Printable(gameTitle.toLower()));
        if((gameTitle.toLower().trimmed()==gameName.toLower().trimmed())||(gameTitle2.toLower().trimmed()==gameName.toLower().trimmed()))
        {
            qDebug(qUtf8Printable(gameTitle));
            qDebug(qUtf8Printable(id));
            DbManager db("data.db");
            if (db.checkAddGamePossible(id.toInt()))
            {
                if (gameSteamid>0)
                {
                    ListedPCGame *pcgame = new ListedPCGame(this,gameTitle.trimmed(),id.toInt(),true,gameSteamid);
                    dialog->addGame(pcgame);
                }
                else
                {
                    ListedPCGame *pcgame = new ListedPCGame(this,gameTitle.trimmed(),id.toInt(),false);
                    dialog->addGame(pcgame);
                }
            }
        }
        delete reply;
    }
    else
    {
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
}
}



BOOL MainWindow::StaticEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    MainWindow *pThis = reinterpret_cast<MainWindow*>(lParam);
    return pThis->EnumWindowsProc(hwnd);
}

BOOL MainWindow::EnumWindowsProc(HWND hwnd)
{
    WCHAR title[255];
    if(GetWindowText(hwnd, title, 255))
        qDebug(qUtf8Printable((QString::fromWCharArray(title))));  //LIST ALL RUNNING PROGRAMS WINDOWS !!!
    return TRUE;
}
//EnumWindows(StaticEnumWindowsProc, reinterpret_cast<LPARAM>(this)); how to use it

void MainWindow::addGameSearched(SearchGame* that)
{
    QString gameName = that->getGameName();
    QString platform = that->getPlatform();
    int id = that->getGameId();
    qDebug(qUtf8Printable(gameName));
    qDebug(qUtf8Printable(platform));
    qDebug(qUtf8Printable(QString::number(id)));
    addGame(gameName,platform,id);
}

void MainWindow::addPCAskedGames(QList<QString>* list)
{
    qDebug(qUtf8Printable("hi2"));
    int index = list->length();
    for(int i=0;i<index;i++)
    {
        QString gameinfo = list->at(i);
        QString gamename = gameinfo.split("{}").at(0);
        QString platform = gameinfo.split("{}").at(1);
        QString id = gameinfo.split("{}").at(2);
        QString checked = gameinfo.split("{}").at(3);
        QString steamid = gameinfo.split("{}").at(4);
        if(checked.toInt())
        {
            qDebug(qUtf8Printable("hi3"));
            addGame(gamename.trimmed(),platform.trimmed(),id.toInt(),steamid.toInt());
        }
    }
    list->clear();
}

void MainWindow::updateSmoothScrolling(bool state)
{
    qDebug(qUtf8Printable("hello lol"));
    if(state)
    {
        ui->listWidgetAllGames->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        ui->listWidgetAllGames->verticalScrollBar()->setSingleStep(15);

        ui->listWidgetSearchGames->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        ui->listWidgetSearchGames->verticalScrollBar()->setSingleStep(15);
    }
    else
    {
        ui->listWidgetAllGames->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        ui->listWidgetSearchGames->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    }
}
