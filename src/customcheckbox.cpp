#include "customcheckbox.h"

CustomCheckBox::CustomCheckBox(QString data, QString text, QWidget *parent) : QCheckBox(parent)
{
    setText(text);
    m_data = data;
    QFont f( "Arial", 10, QFont::Bold);
    setFont(f);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(emitCustomStateChanged(int)));
}

void CustomCheckBox::emitCustomStateChanged(int state)
{
    emit customStateChanged(state,m_data);
}

QString CustomCheckBox::getData(){
    return m_data;
}

QString CustomCheckBox::getInfo()
{
    return m_info;
}

void CustomCheckBox::setInfo(QString info)
{
    m_info = info;
}
