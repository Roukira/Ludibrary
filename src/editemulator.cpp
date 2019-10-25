#include "editemulator.h"
#include "ui_editemulator.h"

EditEmulator::EditEmulator(QString emulatorName, QStringList platforms, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditEmulator)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    DbManager db("data.db");
    m_emulatorName = emulatorName;
    m_platforms = platforms;
    ui->labelTitle->setText("Edit "+emulatorName);
    ui->lineEditEmulatorPath->setText(db.getEmulatorPath(emulatorName));
    initSettingsTab();
    initCheckBoxes();
}

EditEmulator::~EditEmulator()
{
    delete ui;
}

void EditEmulator::on_buttonApply_pressed()
{
    DbManager db("data.db");
    db.setEmulatorPath(m_emulatorName,ui->lineEditEmulatorPath->text());
    //emit updateEmulatorSettings();
    this->close();
}

void EditEmulator::on_buttonCancel_pressed()
{
    this->close();
}

void EditEmulator::on_buttonExit_pressed()
{
    this->close();
}

void EditEmulator::on_buttonSearchEmulatorPath_pressed()
{
    QFileDialog dialog;
    QString exec = dialog.getOpenFileName(this, "Select emulator path");
    if (!exec.isEmpty())
    {
        ui->lineEditEmulatorPath->setText(exec);
    }
}

void EditEmulator::on_buttonSearchImagePath_pressed()
{
    QFileDialog dialog;
    QString exec = dialog.getOpenFileName(this, "Select emulator image path");
    if (!exec.isEmpty())
    {
        ui->lineEditImagePath->setText(exec);
    }
}

void EditEmulator::initCheckBoxes()
{
    DbManager db("data.db");
    foreach (QString platform,m_platforms)
    {
        QCheckBox* checkBox = new QCheckBox(platform);
        m_checkBoxesList.append(checkBox);
        connect(checkBox,SIGNAL(pressed()),this,SLOT(checkBoxUpdate()));
        QListWidgetItem* widget = new QListWidgetItem;
        widget->setSizeHint(checkBox->sizeHint());
        ui->listWidgetCheckBoxes->addItem(widget);
        ui->listWidgetCheckBoxes->setItemWidget(widget,checkBox);
        if(db.getPlatformEmulator(platform)==m_emulatorName)
        {
            checkBox->setChecked(true);
        }
    }
}

void EditEmulator::checkBoxUpdate()
{
    DbManager db("data.db");
    QString platform;
    foreach (QCheckBox* checkbox, m_checkBoxesList)
    {
        platform = checkbox->text();
        if (checkbox->isChecked())
        {
            db.setPlatformEmulator(platform,m_emulatorName);

        }
        else
        {
            bool defaultEmulatorExists = false;
            QStringList list = db.getAllEmulators();
            if (list.contains(db.PlatformToSupportedEmulator(platform)))
            {
                defaultEmulatorExists = true;
            }
            if(defaultEmulatorExists)
            {
                db.setPlatformEmulator(platform,db.PlatformToSupportedEmulator(platform));
            }
        }
    }
}

void EditEmulator::initSettingsTab()
{
    DbManager db("data.db");
    bool changesMade = false;
    foreach (QString platform, m_platforms)
    {
        if (db.PlatformToSupportedEmulator(platform)==m_emulatorName)
        {
            changesMade = true;
            QLabel* labelPlatform = new QLabel(platform,this);
            ui->layoutSettings->addWidget(labelPlatform);
            //add checkBoxes and stuff after finding all general settings and each custom settings for each platform (might need a new m_list specific for this case and therefore a new func)

        }
    }
    if (!changesMade)
    {
        ui->tabSettings->hide();
    }
}
