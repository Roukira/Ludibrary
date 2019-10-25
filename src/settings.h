#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QScreen>
#include <QListWidgetItem>
#include <QFile>
#include <QSettings>
#include <QScrollBar>
#include <QFileDialog>
#include <QList>
#include <QCheckBox>
#include <QPair>
#include <QMessageBox>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void resizeWindow();
    bool getState();
    void setState(bool state);
    void loadSettings();
    bool getDeletePlatform();
    QString getEmulatorsDir();
    bool getNewGameDesign();
    void setNewGameDesign(bool state);

private slots:
    void on_buttonExit_clicked();
    void on_checkBoxSmoothScrolling_toggled(bool state);
    void on_checkBoxDeletePlatform_toggled(bool state);
    void on_checkBoxGameDesign_toggled(bool state);
    void on_buttonEmulatorsDir_pressed();
    void tabChange();

signals:
    void smoothScrolling(bool state);

private:
    Ui::Settings *ui;
    bool m_wasActive = false;

    bool setting_smoothScrolling;
    bool setting_deletePlatform;
    bool setting_newGameDesign;
    QString setting_emulatorsDir;

    bool infoPopup = true;
};

#endif // SETTINGS_H
