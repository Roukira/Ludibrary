#ifndef ADDEMULATORDIALOG_H
#define ADDEMULATORDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QComboBox>
#include <QList>
#include <dbmanager.h>
#include <emulator.h>

namespace Ui {
class AddEmulatorDialog;
}

class AddEmulatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmulatorDialog(QWidget *parent = 0);
    ~AddEmulatorDialog();
    Emulator* getEmulator();
    void showUp();

private slots:
    void on_buttonAdd_pressed();
    void on_buttonCancel_pressed();
    void on_buttonExit_pressed();
    void on_buttonSearchPath_pressed();
    void on_buttonAddPlatform_pressed();

private:
    Ui::AddEmulatorDialog *ui;
    int m_nextPlatformRow = 3;
    QStringList m_platforms;
    QList<QComboBox*> m_newBoxes;
    bool m_ok;
};

#endif // ADDEMULATORDIALOG_H
