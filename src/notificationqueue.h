#ifndef NOTIFICATIONQUEUE_H
#define NOTIFICATIONQUEUE_H

#include <QObject>
#include <QQueue>
#include <notification.h>

class NotificationQueue : public QObject
{
    Q_OBJECT
public:
    explicit NotificationQueue(QObject *parent = nullptr);
    void setCurrentMainWindowSize(QSize size);

private slots:
    void notificateUser();
    void queueNotification(Notification* notif);
    void unqueueNotification();

private:
    QQueue<Notification*> m_notifQueue;
    bool m_queueNotEmpty = false;
    QSize m_currentMainWindowSize;
};

#endif // NOTIFICATIONQUEUE_H
