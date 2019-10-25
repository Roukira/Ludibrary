#include "bubbletext.h"

BubbleText::BubbleText(QWidget* parent) : QTextEdit(parent)
{
    this->setAttribute(Qt::WA_Hover,true);
}

void BubbleText::wheelEvent(QWheelEvent *event)
{
    qDebug(qUtf8Printable("wheeling in bubbletext"));
    QTextEdit::wheelEvent(event);
}
