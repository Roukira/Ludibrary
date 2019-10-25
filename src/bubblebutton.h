#ifndef BUBBLEBUTTON_H
#define BUBBLEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QHoverEvent>
#include <QDebug>
#include <QScrollBar>
#include <bubbletext.h>
#include <QCursor>

class MainWindow;

class BubbleButton : public QPushButton
{

    Q_OBJECT

public:
    BubbleButton(int w, int h, QString info, QWidget* parent = 0);
    void hoverEnters(QHoverEvent * event);
    void hoverLeaves(QHoverEvent * event);
    bool event(QEvent * e);
    QString getInfoBubble();
    void setInfoBubble(QString info);
    BubbleText* createBubbleWidget();
    BubbleText* getWidget();
    static MainWindow* getMainWindow();
private:
    QString m_infoBubble;
    BubbleText* m_bubbleWidget;
    MainWindow* m_mainwindow;
    QSize m_bubbleSize;
};

#endif // BUBBLEBUTTON_H
