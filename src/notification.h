#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QTimer>
#include <dbmanager.h>

namespace Ui {
class Notification;
}

class Notification : public QWidget
{
    Q_OBJECT

public:
    explicit Notification(QString text = "", int gameId = -1,QWidget *parent = 0);
    ~Notification();
    void notificate();
    void askForPlacement(QSize size);
    void hideNotification();

private slots:

    void placeNotification(QSize size);

private:
    Ui::Notification *ui;

};

#endif // NOTIFICATION_H
