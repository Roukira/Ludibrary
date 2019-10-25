#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QScreen>
#include <QListWidgetItem>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class Help;
}

class Help : public QWidget
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();
    void resizeWindow();
    bool getState();
    void setState(bool state);

private slots:
    void on_buttonExit_clicked();
    void tabChange();

private:
    Ui::Help *ui;
    bool m_wasActive = false;
};

#endif // HELP_H
