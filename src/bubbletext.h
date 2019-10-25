#ifndef BUBBLETEXT_H
#define BUBBLETEXT_H

#include <QObject>
#include <QWidget>
#include <QEvent>
#include <QTextEdit>
#include <QDebug>
#include <QWheelEvent>

class BubbleText : public QTextEdit
{
    Q_OBJECT
public:
    BubbleText(QWidget* parent = 0);
    void wheelEvent(QWheelEvent *event);

};

#endif // BUBBLETEXT_H
