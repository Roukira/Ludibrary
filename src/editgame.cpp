#include "mainwindow.h"
#include "editgame.h"
#include "ui_editgame.h"

editGame::editGame(int id, QString gameName, QString platform, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editGame)
{
    ui->setupUi(this);
    DbManager db("data.db");
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->labelTitle->setText("Edit "+gameName);
    m_path = db.getGamePath(id);
    ui->lineEditPath->setText(m_path);
    m_id = id;
    m_platform = platform;
    updateExeList();
    m_state = db.getGameState(m_id);
    if(m_state.isEmpty())
    {
        m_state = "Never Played";
        db.setGameState(id,m_state);
    }
    if(m_state == "Never Played") ui->gameStateComboBox->setCurrentIndex(0);
    if(m_state == "Plan to Play") ui->gameStateComboBox->setCurrentIndex(1);
    if(m_state == "Dropped") ui->gameStateComboBox->setCurrentIndex(2);
    if(m_state == "Completed") ui->gameStateComboBox->setCurrentIndex(3);
    if(m_state == "Regularly Playing") ui->gameStateComboBox->setCurrentIndex(4);
}

void editGame::updateState()
{
    DbManager db("data.db");
    QString state = ui->gameStateComboBox->currentText();
    m_state = state;
    db.setGameState(m_id,state);
    emit updateParentState();
}

editGame::~editGame()
{
    delete ui;
}


void editGame::on_buttonApply_pressed()
{
    DbManager db("data.db");
    db.setGamePath(m_id,ui->lineEditPath->text());
    m_path = ui->lineEditPath->text();
    updateExeList();
    updateState();
    this->close();
}

void editGame::on_buttonCancel_pressed()
{
    this->close();
}

void editGame::on_buttonExit_pressed()
{
    this->close();
}

void editGame::on_buttonSearchPath_pressed()
{
    QFileDialog dialog;
    QString exec = dialog.getOpenFileName(this, "Select game launcher file", QString(),QString(),0,QFileDialog::DontResolveSymlinks);

    qDebug(qUtf8Printable(exec));
    qDebug(qUtf8Printable("lul"));
    if (!exec.isEmpty())
    {
        ui->lineEditPath->setText(exec);
    }
}

void editGame::updateExeList()
{
    QStringList nameFilter("*.exe");
    QDir directory(QFile::symLinkTarget(m_path));
    directory.cdUp();
    qDebug(qUtf8Printable("dir : "+directory.path()));
    QStringList exeList;
    QDirIterator it(directory.path(), nameFilter, QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        exeList.append(it.next().split("/").last());
    }
    qDebug(qUtf8Printable("printing exe qstringlist"));
    foreach(QString text,exeList)
    {
        qDebug(qUtf8Printable(text));
    }
    m_exeList = exeList;
}

QStringList editGame::getExeList()
{
    return m_exeList;
}

void editGame::addGameToBlacklist(QString extSoftName)
{
    DbManager db("data.db");
    QList<int> gamesOff = db.getExternalSoftwareGamesOffList(extSoftName);
    if(gamesOff.contains(m_id))
    {
        return;
    }
    else
    {
        gamesOff.append(m_id);
        db.setExternalSoftwareGamesOffList(extSoftName,gamesOff);
    }
    emit getMainWindow()->updateExternalSoftwareExceptions();
}

void editGame::removeGameFromBlacklist(QString extSoftName)
{
    DbManager db("data.db");
    QList<int> gamesOff = db.getExternalSoftwareGamesOffList(extSoftName);
    if(gamesOff.contains(m_id))
    {
        gamesOff.removeOne(m_id);
        db.setExternalSoftwareGamesOffList(extSoftName,gamesOff);
    }
    emit getMainWindow()->updateExternalSoftwareExceptions();
}

void editGame::addGameToWhitelist(QString extSoftName){
    DbManager db("data.db");
    QList<int> gamesWhitelist = db.getExternalSoftwareGamesWhitelist(extSoftName);
    if(gamesWhitelist.contains(m_id))
    {
        return;
    }
    else
    {
        gamesWhitelist.append(m_id);
        db.setExternalSoftwareGamesWhitelist(extSoftName,gamesWhitelist);
    }
    emit getMainWindow()->updateExternalSoftwareExceptions();
}

void editGame::removeGameFromWhitelist(QString extSoftName){
    DbManager db("data.db");
    QList<int> gamesWhitelist = db.getExternalSoftwareGamesWhitelist(extSoftName);
    if(gamesWhitelist.contains(m_id))
    {
        gamesWhitelist.removeOne(m_id);
        db.setExternalSoftwareGamesWhitelist(extSoftName,gamesWhitelist);
    }
    emit getMainWindow()->updateExternalSoftwareExceptions();
}

void editGame::addExternalSoftwareWidget(QString extSoftName){
    DbManager db("data.db");
    bool gameAllowed = db.getExternalSoftwareGameCanRun(extSoftName,m_id);

    CustomCheckBox* checkbox = new CustomCheckBox(extSoftName, "Run "+extSoftName+".");
    checkbox->setChecked(gameAllowed);
    connect(checkbox,SIGNAL(customStateChanged(bool,QString)),this,SLOT(updateGameState(bool,QString)));
    ui->verticalLayout_3->addWidget(checkbox);
    m_checkboxlist.append(checkbox);
}

void editGame::updateGameState(bool state, QString extSoftName){
    DbManager db("data.db");
    bool gameswhitelisted = db.getExternalSoftwareGamesWhitelist(extSoftName).contains(m_id);
    bool platformon = db.getExternalSoftwarePlatformsOnList(extSoftName).contains(db.getGamePlatform(m_id));
    if (state){
        if(!platformon){
            addGameToWhitelist(extSoftName);
            return;
        }
        removeGameFromBlacklist(extSoftName);
        return;
    }
    if(gameswhitelisted){
        removeGameFromWhitelist(extSoftName);
        return;
    }
    addGameToBlacklist(extSoftName);

}

bool editGame::IsExternalSoftwareLoaded(QString extSoftName){
    foreach(CustomCheckBox* ccbox, m_checkboxlist){
        if(ccbox->getData()==extSoftName){
            return true;
        }
    }
    return false;
}

MainWindow* editGame::getMainWindow()
{
    foreach(QWidget *widget, qApp->topLevelWidgets())
        if (MainWindow *mainWindow = qobject_cast<MainWindow*>(widget))
            return mainWindow;
    return NULL;
}
