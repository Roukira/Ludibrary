#ifndef EXTERNALSOFTWARE_H
#define EXTERNALSOFTWARE_H

#include <QObject>
#include <QList>
#include "customcheckbox.h"
#include <QVBoxLayout>
#include <QLabel>
#include <windows.h>
#include "dbmanager.h"

class ExternalSoftware : public QObject
{
    Q_OBJECT
public:
    explicit ExternalSoftware(QString externalSoftwareName, QString path = "", QStringList platforms = QStringList(), bool supported = false, QObject *parent = nullptr);
    void incrementOpenedCounter();
    void setSoftwarePath(QString path);
    QWidget* getWidget();

    void defaultPid();
    void setPid(qint64 pid);
    bool isRunning();
    QString getName();
    bool getMultiInstance();

signals:

public slots:

    void addGameToBlacklist(int gameId);
    void removeGameFromBlacklist(int gameId);
    void addPlatformToWhitelist(QString platform);
    void removePlatformFromWhitelist(QString platform);
    void updatePlatformsFromCheckBox(bool state, QString data);
    void updateExceptions();
    void updateMultiInstance(bool state);

private:

    void createSettingsWidget();

    QString m_extsoftname;
    QString m_path;
    bool m_supported;
    bool m_multiInstance;
    int m_openedCounter;

    //use these to check which games to open with this software. Create public slot to update this function when signal emitted by settings or game settings.
    QStringList m_platformsOn;
    QList<int> m_gamesOff;

    QWidget* m_settingsWidget;
    QList<CustomCheckBox*> m_checkboxlist;
    QList<QLabel*> m_infolist;

    qint64 m_pid;



};

#endif // EXTERNALSOFTWARE_H
