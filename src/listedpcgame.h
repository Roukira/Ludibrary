#ifndef LISTEDPCGAME_H
#define LISTEDPCGAME_H

#include <QWidget>

namespace Ui {
class ListedPCGame;
}

class ListedPCGame : public QWidget
{
    Q_OBJECT

public:
    explicit ListedPCGame(QWidget *parent = 0,QString gameName="", int id =-1,bool isSteam=false, int gameSteamid=-1);
    ~ListedPCGame();
    QString getGameName();
    QString getPlatform();
    int getId();
    int getSteamId();
    bool getChecked();

private slots:
    void updateChecked();
private:
    Ui::ListedPCGame *ui;
    QString m_gameName;
    QString m_platform="PC";
    int m_id;
    int m_steamid;
    bool m_checked;
    bool m_isSteam;
};

#endif // LISTEDPCGAME_H
