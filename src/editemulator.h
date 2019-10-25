#ifndef EDITEMULATOR_H
#define EDITEMULATOR_H

#include <QWidget>
#include <QFileDialog>
#include <QCheckBox>
#include <QList>
#include <dbmanager.h>

namespace Ui {
class EditEmulator;
}

class EditEmulator : public QWidget
{
    Q_OBJECT

public:
    explicit EditEmulator(QString emulatorName, QStringList platforms, QWidget *parent = 0);
    ~EditEmulator();
    void initCheckBoxes();
    void initSettingsTab();

signals:
    void custom_toggled(QCheckBox* checkbox, bool state);
    //void updateEmulatorSettings();
private slots:
    void on_buttonApply_pressed();
    void on_buttonCancel_pressed();
    void on_buttonExit_pressed();
    void on_buttonSearchEmulatorPath_pressed();
    void on_buttonSearchImagePath_pressed();
    void checkBoxUpdate();
private:
    Ui::EditEmulator *ui;
    QString m_emulatorName;
    QStringList m_platforms;
    QList<QCheckBox*> m_checkBoxesList;
};

#endif // EDITEMULATOR_H
