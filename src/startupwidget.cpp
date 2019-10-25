#include "startupwidget.h"
#include "ui_startupwidget.h"

StartUpWidget::StartUpWidget(QSplashScreen *parent) :
    QWidget(parent),
    ui(new Ui::StartUpWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    movie = new QMovie(":/Images/KonamiLoading");
    ui->labelGif->setMovie(movie);
    movie->start();

}

StartUpWidget::~StartUpWidget()
{
    delete ui;
    movie->deleteLater();
}

void StartUpWidget::setText(QString text){
    ui->labelText->setText(text);
    process();
}


void StartUpWidget::process(){
    qDebug() << "debug";
    parentWidget()->repaint();
    qApp->processEvents();
}
