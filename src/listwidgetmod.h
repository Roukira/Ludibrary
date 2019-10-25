#ifndef LISTWIDGETMOD_H
#define LISTWIDGETMOD_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QDebug>
#include <QWheelEvent>
#include <bubblebutton.h>

class ListWidgetMod : public QListWidget
{
    Q_OBJECT

public:
    ListWidgetMod(QWidget* parent = 0);
    void wheelEvent(QWheelEvent *event);
};

#endif // LISTWIDGETMOD_H
