#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog();
    int getChoice();


private slots:
    void getCheckedButton();

private:
    QRadioButton *m_steam;
    QRadioButton *m_game;
    QRadioButton *m_emulator;
    QRadioButton *m_extsoft;
    int m_choice;
};

#endif // ADDDIALOG_H
