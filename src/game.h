#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDateTime>
#include <QMenu>
#include <QPixmap>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDesktopServices>
#include <QSettings>
#include <QList>
#include "filedownloader.h"
#include "editgame.h"
#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <bubblebutton.h>

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game();
    explicit Game(QString gameName, QWidget *parent = 0, int id = -1, QString platformType="PC",QString emulatorName="None", int steamid =-1);
    explicit Game(bool mode, QString gameName, QWidget *parent = 0, int id = -1, QString platformType="PC",QString emulatorName="None", int steamid =-1); //
    ~Game();
    void runGame();
    QString getGameName();
    QString getPlatform();
    QString getImagePath();
    int getId();
    int getTimePlayed();
    void showTimePlayed();
    void getImageFromId(int id);
    void updateGameInfo();
    void incrementOpenedCounter();
    void labelSize();
    bool IsProcessRunning(DWORD pid);
    qint64 SearchChild(DWORD pid,HANDLE h, PROCESSENTRY32 pe);
    void addExternalSoftwareWidgetAsk(QString extSoftName);
    bool isExternalSoftwareLoaded(QString extSoftName);
    QString lastTimePlayedToString();
    void initializeBubbleButton();

    void hoverEnter(QHoverEvent * event);
    void hoverLeave(QHoverEvent * event);
    bool event(QEvent * e);

public slots:
    void setupGameWidget();
private slots:

    void updateSettings(QString gameName, QString gamePlatform, int id);
    //void updateGameState(QProcess::ProcessState state);
    void updateGameTimePlayed();
    void changeSettings();
    void gameQuit(int exitStatus);
    void showSearchReply(QNetworkReply* reply);
    void on_buttonPlay_pressed();
    void on_buttonEdit_pressed();
    void startGameTimer();
    void updateLastTimePlayedLabel();
    void updateMState();

signals:
    void newNotificationAsk(int gameId, QString text);
    void updateStatusBarStateAsk(QString state);
    void updateStatusBarProgressAsk(QString progress);
    void updateStatusBarProgressBarAsk(int value);

private:


    Ui::Game *ui;

    QString m_gameName;
    QString m_gamePlatform;
    int m_id;
    int m_steamid;
    int m_openedCounter;
    QString m_emulatorName;
    QString m_gameIconPath;
    QString m_gameBoxArtPath;
    QString m_gameInfo;
    int m_gameTimePlayed = 0;
    int m_gameTimePlayedOnSession = 0;
    QTimer* m_timer;
    QDateTime m_lastplayed;
    QString m_state;
    bool m_gameFavorited;
    bool m_gameInstalled;
    editGame* m_editWindow;
    qint64 m_pid;
    int m_timerInterval = 1000;
    QTimer* m_lastplayedTimer;
    BubbleButton* m_infoButton;

};

#endif // GAME_H
