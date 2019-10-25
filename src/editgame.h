#ifndef EDITGAME_H
#define EDITGAME_H

#include <QWidget>
#include <QFileDialog>
#include <QDirIterator>
#include <dbmanager.h>
#include "customcheckbox.h"

class MainWindow;

namespace Ui {
class editGame;
}

class editGame : public QWidget
{
    Q_OBJECT

public:
    explicit editGame(int id, QString gameName, QString platform, QWidget *parent = 0);
    void updateExeList();
    void addGameToBlacklist(QString extSoftName);
    void removeGameFromBlacklist(QString extSoftName);
    void addGameToWhitelist(QString extSoftName);
    void removeGameFromWhitelist(QString extSoftName);
    void addExternalSoftwareWidget(QString extSoftName);
    bool IsExternalSoftwareLoaded(QString extSoftName);
    void updateState();
    QStringList getExeList();
    static MainWindow* getMainWindow();
    ~editGame();

signals:
    void updateParentState();
private slots:
    void on_buttonApply_pressed();
    void on_buttonCancel_pressed();
    void on_buttonExit_pressed();
    void on_buttonSearchPath_pressed();
    void updateGameState(bool state, QString extSoftName);
private:
    Ui::editGame *ui;
    int m_id;
    QString m_platform;
    QString m_path;
    QStringList m_exeList;
    QString m_state;
    QList<CustomCheckBox*> m_checkboxlist;
};

#endif // EDITGAME_H
