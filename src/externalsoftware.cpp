#include "externalsoftware.h"

ExternalSoftware::ExternalSoftware(QString externalSoftwareName,QString path, QStringList platforms, bool supported ,QObject *parent) : QObject(parent)
{
    DbManager db("data.db");
    if (db.checkAddExternalSoftwarePossible(externalSoftwareName))
    {
        if (db.addExternalSoftwareEntry(externalSoftwareName))
        {
            m_extsoftname = externalSoftwareName;
            m_path = path;
            m_supported = supported;
            m_openedCounter = 0;

            db.setExternalSoftwarePath(externalSoftwareName,m_path);
            db.setExternalSoftwareSupportedBool(externalSoftwareName,m_supported);

            db.setExternalSoftwarePlatformsOnList(externalSoftwareName,platforms);
            m_platformsOn = platforms;
            m_multiInstance = false;
            db.setExternalSoftwareMutliInstance(externalSoftwareName,m_multiInstance);
        }

    }
    else
    {
        m_extsoftname = externalSoftwareName;
        m_path = db.getExternalSoftwarePath(externalSoftwareName);
        m_supported = db.getExternalSoftwareSupportedBool(externalSoftwareName);
        m_openedCounter = db.getExternalSoftwareOpenedCounter(externalSoftwareName);
        m_gamesOff = db.getExternalSoftwareGamesOffList(externalSoftwareName);
        m_platformsOn = db.getExternalSoftwarePlatformsOnList(externalSoftwareName);
        m_multiInstance = db.getExternalSoftwareMutliInstance(externalSoftwareName);
    }
    createSettingsWidget();
    defaultPid();
}

void ExternalSoftware::createSettingsWidget(){

    DbManager db("data.db");
    QStringList platsAllowed = db.getExternalSoftwarePlatformsOnList(m_extsoftname);
    QWidget* widget1 = new QWidget();
    widget1->setStyleSheet(".QWidget{"
                           "background-color: transparent;"
                           "}");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setSizeConstraint(layout->SetMinimumSize);
    widget1->setLayout(layout);
    QFont f( "Arial", 18, QFont::Bold);
    QFont f2( "Arial", 8);
    QFont f3( "Arial", 10, QFont::Bold);
    f2.setItalic(true);
    QLabel* labelName = new QLabel(m_extsoftname);
    labelName->setFont(f);
    widget1->layout()->addWidget(labelName);
    QSpacerItem *spacer = new QSpacerItem(1,40,QSizePolicy::Expanding);
    layout->addSpacerItem(spacer);
    QCheckBox *multi = new QCheckBox("Allow to run multiple instances.");
    multi->setChecked(m_multiInstance);
    multi->setFont(f3);
    connect(multi,SIGNAL(toggled(bool)),this,SLOT(updateMultiInstance(bool)));
    widget1->layout()->addWidget(multi);
    foreach(QString platform, db.getAllPlatforms()){
        CustomCheckBox* checkbox = new CustomCheckBox(platform, "Runs with "+platform+" games.");
        QLabel *info = new QLabel;
        info->setFont(f2);
        info->setStyleSheet("QLabel{ color : DarkGrey;}");
        QList<QString> list;
        foreach (int id,db.getExternalSoftwareGamesOffList(m_extsoftname))
        {
            if (db.getGamePlatform(id)==platform){
                list.append(db.getGameTitle(id));
            }
        }
        checkbox->setChecked(platsAllowed.contains(platform));
        checkbox->setInfo("Exceptions : "+list.join(","));
        info->setText(checkbox->getInfo());
        connect(checkbox,SIGNAL(customStateChanged(bool,QString)),this,SLOT(updatePlatformsFromCheckBox(bool,QString)));
        m_checkboxlist.append(checkbox);
        m_infolist.append(info);
        widget1->layout()->addWidget(checkbox);
        widget1->layout()->addWidget(info);
    }
    QSpacerItem *item = new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Expanding);
    layout->addSpacerItem(item);
    m_settingsWidget = widget1;
    updateExceptions();

}

void ExternalSoftware::updateMultiInstance(bool state){
    m_multiInstance = state;
    DbManager db("data.db");
    db.setExternalSoftwareMutliInstance(m_extsoftname,state);
}

QWidget* ExternalSoftware::getWidget(){
    return m_settingsWidget;
}

void ExternalSoftware::defaultPid(){
    m_pid = -1;
}

bool ExternalSoftware::isRunning(){
    if(m_pid==-1) return false;
    HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, m_pid);
    DWORD ret = WaitForSingleObject(process, 0);
    CloseHandle(process);
    return ret == WAIT_TIMEOUT;
}

QString ExternalSoftware::getName(){
    return m_extsoftname;
}

bool ExternalSoftware::getMultiInstance(){
    return m_multiInstance;
}
void ExternalSoftware::setPid(qint64 pid){
    m_pid = pid;
}

void ExternalSoftware::incrementOpenedCounter()
{
    DbManager db("data.db");
    m_openedCounter++;
    db.setExternalSoftwareOpenedCounter(m_extsoftname,m_openedCounter);
}

void ExternalSoftware::setSoftwarePath(QString path)
{
    DbManager db("data.db");
    db.setExternalSoftwarePath(m_extsoftname,path);
    m_path = path;

}


void ExternalSoftware::addGameToBlacklist(int gameId)
{
    DbManager db("data.db");
    if(m_gamesOff.contains(gameId))
    {
        return;
    }
    else
    {
        m_gamesOff.append(gameId);
        db.setExternalSoftwareGamesOffList(m_extsoftname,m_gamesOff);
    }
}

void ExternalSoftware::removeGameFromBlacklist(int gameId)
{
    DbManager db("data.db");
    if(m_gamesOff.contains(gameId))
    {
        m_gamesOff.removeOne(gameId);
        db.setExternalSoftwareGamesOffList(m_extsoftname,m_gamesOff);
    }
}

void ExternalSoftware::addPlatformToWhitelist(QString platform)
{
    DbManager db("data.db");
    if(m_platformsOn.contains(platform))
    {
        return;
    }
    else
    {
        m_platformsOn.append(platform);
        db.setExternalSoftwarePlatformsOnList(m_extsoftname,m_platformsOn);
    }
}

void ExternalSoftware::removePlatformFromWhitelist(QString platform)
{
    DbManager db("data.db");
    if(m_platformsOn.contains(platform))
    {
        m_platformsOn.removeOne(platform);
        db.setExternalSoftwarePlatformsOnList(m_extsoftname,m_platformsOn);
    }
}

void ExternalSoftware::updatePlatformsFromCheckBox(bool state, QString data){

    if(state){
        addPlatformToWhitelist(data);
    }
    else{
        removePlatformFromWhitelist(data);
    }

}

void ExternalSoftware::updateExceptions(){
    DbManager db("data.db");
    int i = 0;
    foreach (QString platform, db.getAllPlatforms()){

        QList<QString> list;
        if(db.getExternalSoftwarePlatformsOnList(m_extsoftname).contains(platform)){
            foreach (int id,db.getExternalSoftwareGamesOffList(m_extsoftname))
            {
                if (db.getGamePlatform(id)==platform){
                    list.append(db.getGameTitle(id));
                }
            }
        }
        else{
            foreach (int id,db.getExternalSoftwareGamesWhitelist(m_extsoftname))
            {
                if (db.getGamePlatform(id)==platform){
                    list.append(db.getGameTitle(id));
                }
            }
        }
        m_infolist.at(i)->setText("Exceptions : "+list.join(","));
        i++;
    }
}
