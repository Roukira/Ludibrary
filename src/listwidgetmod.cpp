#include <mainwindow.h>
#include "listwidgetmod.h"

ListWidgetMod::ListWidgetMod(QWidget* parent) : QListWidget(parent)
{

}

void ListWidgetMod::wheelEvent(QWheelEvent *event)
{
    qDebug(qUtf8Printable("wheeling in listwidgetmod"));
    BubbleText* bubble = BubbleButton::getMainWindow()->getActiveBubble();
    if(bubble) bubble->wheelEvent(event);
    else QListWidget::wheelEvent(event);
}
