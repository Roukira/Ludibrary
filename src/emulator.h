#ifndef EMULATOR_H
#define EMULATOR_H

#include <QWidget>
#include <QDir>
#include <editemulator.h>

namespace Ui {
class Emulator;
}

class Emulator : public QWidget
{
    Q_OBJECT

public:
    explicit Emulator(QString path, QString name, QStringList platforms,QWidget *parent = 0);
    ~Emulator();
    void hide_image();
    void hide_edit();
    QString getPath();
    QString getName();
    QStringList getPlatforms();

private slots:
    void on_buttonEdit_pressed();

private:
    Ui::Emulator *ui;
    QString m_path;
    QString m_name;
    QStringList m_platforms;
    EditEmulator* m_editWindow;
};

#endif // EMULATOR_H
