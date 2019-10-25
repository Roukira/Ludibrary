#include "searchgame.h"
#include "ui_searchgame.h"

SearchGame::SearchGame(QWidget *parent, QString gameName, QString platform, QString releaseDate, int id) :
    QWidget(parent),
    ui(new Ui::SearchGame)
{
    ui->setupUi(this);
    setGame(gameName,platform,releaseDate, id);
}

SearchGame::~SearchGame()
{
    delete ui;
}

void SearchGame::setGame(QString gameName, QString platform, QString releaseDate, int id)
{
    m_gameName = gameName;
    ui->labelGameName->setText(gameName);
    m_platform = platform;
    ui->labelPlatform->setText(platform);
    m_releaseDate = releaseDate;
    ui->labelReleaseDate->setText(releaseDate);
    if(id>-1)
    {
        m_id = id;
    }
}

void SearchGame::on_buttonAdd_clicked()
{
    emit askAddGame(this);
}

QString SearchGame::getGameName()
{
    return m_gameName;
}

QString SearchGame::getPlatform()
{
    return m_platform;
}

int SearchGame::getGameId()
{
    return m_id;
}
