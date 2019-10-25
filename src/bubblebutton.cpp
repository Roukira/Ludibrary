#include "mainwindow.h"
#include "bubblebutton.h"

BubbleButton::BubbleButton(int w, int h, QString info, QWidget* parent) : QPushButton(parent)
{
    setMinimumHeight(h);
    setMaximumHeight(h);
    setMinimumWidth(w);
    setMaximumWidth(w);
    m_infoBubble = info;
    MainWindow* mainwindow = getMainWindow();
    if(!mainwindow){
        qDebug(qUtf8Printable("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"));
        exit(1);
    }
    m_mainwindow = mainwindow;
    setStyleSheet("QPushButton\n{\n    background-color: transparent;\n	background-image: url(:/Images/buttonInfo);\n    background-repeat: none;    \n    border: none;\n};");
    m_bubbleWidget = createBubbleWidget();
}


BubbleText* BubbleButton::createBubbleWidget(){

    BubbleText *widget1 = new BubbleText(m_mainwindow);
    widget1->hide();
    widget1->setReadOnly(true);
    QFont f( "Arial", 12, QFont::Bold);
    widget1->setFont(f);
    //widget1->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //widget1->setAttribute(Qt::WA_TranslucentBackground);
    int widgetResX = m_mainwindow->size().width()/3;
    int widgetResY = m_mainwindow->size().height()/3;
    m_bubbleSize = QSize(widgetResX,widgetResY);
    widget1->resize(widgetResX,widgetResY);
    widget1->setStyleSheet("QTextEdit\n{\n	border-style : solid;"
                           "\nborder-width : 3px; border-radius : 15px; border-color : black;\n"
                           "background : transparent; background-image : url(:/Images/new_background2);\n};");
    widget1->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{\nborder: 1px solid #999999;\nbackground:transparent;"
                                                  "\nwidth:10px;\nmargin: 0px 0px 0px 0px;\n}\n\nQScrollBar::handle:vertical "
                                                  "{\nbackground: qlineargradient(x1:0, y1:0, x2:1, y2:0,stop: 0 rgb(32, 47, 130),"
                                                  " stop: 0.5 rgb(32, 47, 130), stop:1 rgb(32, 47, 130));\nmin-height: 0px;\n}"
                                                  "\n\nQScrollBar::add-line:vertical {\nbackground: qlineargradient(x1:0, y1:0, "
                                                  "x2:1, y2:0,stop: 0 rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb"
                                                  "(32, 47, 130));\nheight: 0px;\nsubcontrol-position: bottom;\nsubcontrol-origin: margin;\n}"
                                                  "\n\nQScrollBar::sub-line:vertical {\nbackground: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
                                                  "stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));\n"
                                                  "height: 0 px;\nsubcontrol-position: top;\nsubcontrol-origin: margin;\n};");
    //widget1->hide();
    widget1->setText(m_infoBubble);
    return widget1;
}

MainWindow* BubbleButton::getMainWindow()
{
    foreach(QWidget *widget, qApp->topLevelWidgets())
        if (MainWindow *mainWindow = qobject_cast<MainWindow*>(widget))
            return mainWindow;
    return NULL;
}

void BubbleButton::hoverEnters(QHoverEvent * event)
{
    qDebug() << Q_FUNC_INFO << this->objectName();
    if(m_mainwindow->getActiveBubble()){
        m_mainwindow->getActiveBubble()->hide();
    }
    m_mainwindow->setActiveBubble(m_bubbleWidget);
    int widgetResX = m_mainwindow->width()/3;
    int widgetResY = m_mainwindow->height()/3;
    m_bubbleSize = QSize(widgetResX,widgetResY);
    QPoint globalPos = this->mapToGlobal(this->rect().topLeft());
    int widgetPosX = globalPos.x()+64;
    qDebug(qUtf8Printable("pos x : " + QString::number(widgetPosX)));
    int widgetPosY = globalPos.y()+64;
    qDebug(qUtf8Printable("pos y : " + QString::number(widgetPosY)));
    qDebug(qUtf8Printable("MainWindow x : "+QString::number(m_mainwindow->width())));
    qDebug(qUtf8Printable("MainWindow y : "+QString::number(m_mainwindow->height())));
    if(widgetPosX+m_bubbleSize.width()>=m_mainwindow->width()) widgetPosX -= (m_bubbleSize.width()+96);
    if(widgetPosY+m_bubbleSize.height()>=m_mainwindow->height()) widgetPosY -= (m_bubbleSize.height()+96);
    if(widgetPosX-m_bubbleSize.width()<=0) widgetPosX += (m_bubbleSize.width()/2 -96);
    if(widgetPosY-m_bubbleSize.height()<=0) widgetPosY += (m_bubbleSize.height()/2-96);
    m_bubbleWidget->setGeometry(widgetPosX,widgetPosY,m_bubbleSize.width(),m_bubbleSize.height());
    m_bubbleWidget->show();
    //QCursor::setPos(m_bubbleWidget->mapToGlobal(m_bubbleWidget->rect().center()));



}
void BubbleButton::hoverLeaves(QHoverEvent * event)
{
    qDebug() << Q_FUNC_INFO << this->objectName();
    m_mainwindow->setActiveBubble(NULL);
    m_bubbleWidget->hide();

}

bool BubbleButton::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::HoverEnter:
        hoverEnters(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeaves(static_cast<QHoverEvent*>(e));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

QString BubbleButton::getInfoBubble(){
    return m_infoBubble;
}

BubbleText* BubbleButton::getWidget(){
    return m_bubbleWidget;
}

void BubbleButton::setInfoBubble(QString info){
    m_infoBubble = info;
    m_bubbleWidget->setText(info);
}
