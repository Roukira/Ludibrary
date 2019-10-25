#include "listedpcgame.h"
#include "ui_listedpcgame.h"

ListedPCGame::ListedPCGame(QWidget *parent,QString gameName, int id,bool isSteam,int gameSteamid) :
    QWidget(parent),
    ui(new Ui::ListedPCGame)
{
    ui->setupUi(this);
    m_gameName = gameName;
    ui->labelGameName->setText(m_gameName);
    ui->labelPlatform->setText(m_platform);
    m_id = id;
    m_isSteam = isSteam;
    m_steamid = gameSteamid;
    if (m_isSteam)
    {
        ui->labelLogo->setPixmap(QPixmap(":/Images/LudibraryRessources/steamlogo.png"));
    }
    m_checked = ui->checkBox->isChecked();
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(updateChecked()));

}

ListedPCGame::~ListedPCGame()
{
    delete ui;
}

QString ListedPCGame::getGameName()
{
    return m_gameName;
}

QString ListedPCGame::getPlatform()
{
    return m_platform;
}

int ListedPCGame::getId()
{
    return m_id;
}

int ListedPCGame::getSteamId()
{
    return m_steamid;
}

bool ListedPCGame::getChecked()
{
    return m_checked;
}

void ListedPCGame::updateChecked()
{
    m_checked = ui->checkBox->isChecked();
}
