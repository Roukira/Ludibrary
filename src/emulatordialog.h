#ifndef EMULATORDIALOG_H
#define EMULATORDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QList>

class Emulatordialog : public QDialog
{
    Q_OBJECT

public:
    Emulatordialog();

private slots:
    void getCheckedButton();

private:
    QList<int> m_checkedEmulators;
    int m_choice;

};

#endif // EMULATORDIALOG_H
