#ifndef ADDEXTERNALSOFTWAREDIALOG_H
#define ADDEXTERNALSOFTWAREDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QComboBox>
#include <QList>
#include <dbmanager.h>
#include <externalsoftware.h>

namespace Ui {
class AddExternalSoftwareDialog;
}

class AddExternalSoftwareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddExternalSoftwareDialog(QWidget *parent = 0);
    ~AddExternalSoftwareDialog();
    void updateExternalSoftware();
    void clear();
    void showUp();

signals:
    void acceptedSoftware(QString name, QString path, QStringList platforms);
private slots:
    void on_buttonAdd_pressed();
    void on_buttonCancel_pressed();
    void on_buttonExit_pressed();
    void on_buttonSearchPath_pressed();
    void on_buttonAddPlatform_pressed();

private:
    Ui::AddExternalSoftwareDialog *ui;
    int m_nextPlatformRow = 3;
    QString m_extSoftName;
    QString m_extSoftPath;
    QStringList m_platforms;
    QStringList m_platformsAccepted;
    QList<QComboBox*> m_newBoxes;
    bool m_ok;
};

#endif // ADDEXTERNALSOFTWAREDIALOG_H
