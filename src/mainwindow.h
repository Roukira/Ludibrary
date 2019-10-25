#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QFileDialog>
#include <QMouseEvent>
#include <QSystemTrayIcon>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QVariant>
#include <QStringList>
#include <QRegExp>
#include <QObject>
#include <QScreen>
#include <QCheckBox>
#include <QSpacerItem>
#include <QProgressBar>
#include "game.h"
#include "emulator.h"
#include "adddialog.h"
#include "settings.h"
#include "searchgame.h"
#include "help.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QGraphicsDropShadowEffect>
#include <windows.h>
#include <listedpcgamedialog.h>
#include <QScrollBar>
#include <QMovie>
#include <QDirIterator>
#include <listemulatorsdialog.h>
#include <notificationqueue.h>
#include <statsmonitor.h>
#include <addexternalsoftwaredialog.h>
#include "startupwidget.h"
#include <QSplashScreen>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addGame(QString gameName = "", QString gamePlatform = "", int id = -1, int steamid = -1);
    //void addGameManually();
    void addPCGame(QString gameName = "",int gameSteamid = -1, ListedPCGameDialog* dialog=0);
    void addTag(QString tagName);
    void loadTag(QString platform);
    void addEmulator();
    void loadAllTags();
    void loadAllExternalSoftwares();
    void loadGame(Game *gameWidget);
    void loadAllGames();
    void initSupportedEmulators();
    void resizeWindow();
    void setSysTray();
    void setStartUp();
    void startApp(StartUpWidget* start);
    QStringList searchAllGames();

    void setActiveBubble(BubbleText* bubble);
    BubbleText* getActiveBubble();
    bool eventFilter(QObject *object, QEvent *event);

    static BOOL CALLBACK StaticEnumWindowsProc(HWND hwnd, LPARAM lParam);
    BOOL EnumWindowsProc(HWND hwnd);

    ExternalSoftware* getExternalSoftware(QString extSoftName);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent* event);
    void closeEvent(QCloseEvent *event);

signals:
    void newSearch();
    void newNotification(Notification* that);
    void updateExternalSoftwareExceptions();

private slots:
    void loadAllEmulators();
    void on_buttonAdd_clicked();
    void on_buttonDeleteSelection_clicked();
    void on_buttonExit_clicked();
    void on_buttonHide_clicked();
    void slotActivated(QSystemTrayIcon::ActivationReason);
    void closeApp();
    void on_buttonSettings_clicked();
    void on_buttonHelp_clicked();
    void on_lineEditSearch_textChanged(QString text);
    void showSearchReply(QNetworkReply* reply);
    void on_comboBoxSort_currentIndexChanged(const QString& text);
    void tagSelected(QListWidgetItem* item);
    void addGameSearched(SearchGame* that);
    void addPCAskedGames(QList<QString>* list);
    void addExternalSoftware(QString name, QString path, QStringList platforms);

    void updateGeneralReply(QString games, QString platform, QString emulator, QString playtime, QString extsoft);
    void createPlatformReply(QLabel* platformName, QLabel* platformGameCounter, QLabel* platformTimePlayed, QLabel* platformOpenedCounter);
    void createEmulatorReply(QLabel* emulatorName, QLabel* emulatorGameCounter, QLabel* emulatorTimePlayed, QLabel* emulatorOpenedCounter);
    void createExtSoftReply(QLabel* extSoftName, QLabel* extSoftOpenedCounter);

    void notificationAsked(int gameId=-1, QString text ="");
    void updateSmoothScrolling(bool state);

    void updateStatusBarState(QString state);
    void updateStatusBarProgress(QString progress);
    void updateStatusBarProgressBar(int value);



private:
    QPoint m_Diff;
    Ui::MainWindow *ui;
    QSystemTrayIcon *m_systemTray;
    Settings *m_settings;
    Help *m_help;
    ListedPCGameDialog *m_listDialog;
    ListEmulatorsDialog *m_listEmulatorsDialog;
    AddExternalSoftwareDialog *m_add_extsoft_dialog;
    QNetworkAccessManager *m_mgrSearch;
    QNetworkReply *m_replySearch;
    NotificationQueue *m_notificationQueue;
    StatsMonitor *m_statsmonitor;

    QLabel *m_statusBarState;
    QLabel *m_statusBarProgressLabel;
    QProgressBar *m_statusBarProgressBar;

    QList<Game*> m_gameWidgets;
    QList<ExternalSoftware*> m_extsoftList;
    BubbleText* m_activeBubble = NULL;

};

#endif // MAINWINDOW_H
