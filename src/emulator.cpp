#include "emulator.h"
#include "ui_emulator.h"
#include "dbmanager.h"

Emulator::Emulator(QString path, QString name, QStringList platforms,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Emulator)
{
    ui->setupUi(this);
    m_path = path;

    m_name = name;
    ui->labelEmulatorName->setText(name);

    QString platform = platforms.join(" and ");
    ui->labelPlatformName->setText(platform);
    m_platforms = platforms;

    EditEmulator *editWindow = new EditEmulator(name,platforms);
    m_editWindow = editWindow;

}

Emulator::~Emulator()
{
    delete ui;
}

void Emulator::hide_image()
{
    ui->labelEmulatorImage->hide();
}

void Emulator::hide_edit()
{
    ui->buttonEdit->hide();
}

QString Emulator::getPath()
{
    return m_path;
}

QString Emulator::getName()
{
    return m_name;
}

QStringList Emulator::getPlatforms()
{
    return m_platforms;
}

void Emulator::on_buttonEdit_pressed()
{
    m_editWindow->show();
}
