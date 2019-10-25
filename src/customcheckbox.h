#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include <QCheckBox>

class CustomCheckBox : public QCheckBox
{

    Q_OBJECT
public:
    explicit CustomCheckBox(QString data, QString text = "", QWidget *parent = 0);
    QString getData();
    QString getInfo();
    void setInfo(QString info);

signals:
    void customStateChanged(bool state, QString data);

public slots:
    void emitCustomStateChanged(int state);


private:
    QString m_data;
    QString m_info;
};

#endif // CUSTOMCHECKBOX_H
