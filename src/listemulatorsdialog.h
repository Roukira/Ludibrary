#ifndef LISTEMULATORSDIALOG_H
#define LISTEMULATORSDIALOG_H

#include <QWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <emulator.h>
#include "dbmanager.h"
#include <addemulatordialog.h>

namespace Ui {
class ListEmulatorsDialog;
}

class ListEmulatorsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ListEmulatorsDialog(QWidget *parent = 0);
    ~ListEmulatorsDialog();
    void clearContent();

signals:
    void newEmulator();
private slots:
    void on_buttonAdd_pressed();
    void on_buttonDelete_pressed();
    void on_pushButtonAdd_pressed();
    void on_pushButtonCancel_pressed();
    void on_buttonExit_pressed();
private:
    Ui::ListEmulatorsDialog *ui;
};

#endif // LISTEMULATORSDIALOG_H
