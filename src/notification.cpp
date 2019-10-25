#include "notification.h"
#include "ui_notification.h"

Notification::Notification(QString text, int gameId,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notification)
{
    ui->setupUi(this);
    if (!text.isEmpty())
    {
        if (gameId>=0)
        {
            DbManager db("data.db");
            QPixmap image;
            image.loadFromData(db.getGameImage(gameId));
            ui->labelNotificationImage->setPixmap(image);
            ui->labelNotificationText->setText(text);
        }
        else
        {
            ui->labelNotificationText->setText(text);
            ui->labelNotificationImage->hide();
        }
    }
    this->hide();
}

Notification::~Notification()
{
    delete ui;
}


void Notification::placeNotification(QSize size)
{
    qDebug(qUtf8Printable("place"));
    this->move(size.width()*0.95-(this->width()),size.height()-(this->height()));
}

void Notification::askForPlacement(QSize size)
{
    placeNotification(size);
}

void Notification::notificate()
{
    qDebug(qUtf8Printable("notificate"));
    this->show();
    /*QTimer timer;
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(hide()));
    timer.start(5000);*/
}

void Notification::hideNotification()
{
    this->hide();
}
