#ifndef STARTUPWIDGET_H
#define STARTUPWIDGET_H

#include <QWidget>
#include <QMovie>
#include <QSplashScreen>
#include <QThread>
#include <QDebug>

namespace Ui {
class StartUpWidget;
}

class StartUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StartUpWidget(QSplashScreen *parent = 0);
    ~StartUpWidget();
    void setText(QString text);
public slots:
    void process();
private:
    Ui::StartUpWidget *ui;
    QMovie *movie;
};

#endif // STARTUPWIDGET_H
