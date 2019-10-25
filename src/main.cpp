#include "mainwindow.h"
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory sharedMemory;

    sharedMemory.setKey("TcpServer-Key");
    if(sharedMemory.create(1) == false)
    {
       QMessageBox::warning(NULL, "Nope!", "Ludibrary is already running ! \nWe've shown the window for you,but check out the icon on Notification Area ! \nIf you want to disable this popup, check on settings.");
       a.exit(); // exit already a process running
       return 0;
    }

    QSplashScreen *splash = new QSplashScreen();
    StartUpWidget *start = new StartUpWidget(splash);
    //splash->setStyleSheet("QSplashScreen { background : url(:/Images/new_background2);}");
    splash->resize(350,380);
    splash->move(QApplication::desktop()->screen()->rect().center() - splash->rect().center());
    start->show();
    splash->raise();
    splash->show();
    start->process();
    qDebug() << "starting up!";
    MainWindow w;

    w.startApp(start);
    w.show();
    splash->finish(&w);
    start->deleteLater();
    splash->deleteLater();
    return a.exec();
}
