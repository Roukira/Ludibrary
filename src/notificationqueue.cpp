#include "notificationqueue.h"

NotificationQueue::NotificationQueue(QObject *parent) : QObject(parent)
{
    //everything is ok, but add more notification per action.
}


void NotificationQueue::queueNotification(Notification *notif)
{
    qDebug(qUtf8Printable("queue"));
    bool ok = false;
    if (!m_queueNotEmpty)
    {
        ok = true;
    }
    m_notifQueue.enqueue(notif);
    m_queueNotEmpty = true;
    if (ok)
    {
        qDebug(qUtf8Printable("queue"));
        for(int i=0;i<m_notifQueue.count();i++)
        {
            qDebug(qUtf8Printable(QString::number(i)));
        }
        notificateUser();
    }
}

void NotificationQueue::unqueueNotification()
{
    qDebug(qUtf8Printable("unqueue"));
    Notification* notif = m_notifQueue.dequeue();
    notif->hideNotification();
    delete notif;
    qDebug(qUtf8Printable("unqueue"));
    for(int i=0;i<m_notifQueue.count();i++)
    {
        qDebug(qUtf8Printable(QString::number(i)));
    }
    if (m_notifQueue.isEmpty())
    {
        m_queueNotEmpty = false;
    }
    else
    {
        notificateUser();
    }

}

void NotificationQueue::notificateUser()
{
    qDebug(qUtf8Printable("notificateUser"));
    if(m_queueNotEmpty)
    {
        Notification* notif = m_notifQueue.head();
        notif->askForPlacement(m_currentMainWindowSize);
        notif->notificate();
        QTimer timer;
        timer.singleShot(5000,this,SLOT(unqueueNotification()));
    }
}

void NotificationQueue::setCurrentMainWindowSize(QSize size)
{
    m_currentMainWindowSize = size;
}
