#include "addemulatordialog.h"
#include "ui_addemulatordialog.h"

AddEmulatorDialog::AddEmulatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmulatorDialog)
{
    DbManager db("data.db");
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    QStringList platforms = db.getAllPlatforms();
    m_platforms = platforms;
    ui->comboBoxPlatform->addItems(platforms);
    this->showUp();
    this->exec();
}

AddEmulatorDialog::~AddEmulatorDialog()
{
    delete ui;
}

void AddEmulatorDialog::showUp(){

    DbManager db("data.db");
    m_platforms = db.getAllPlatforms();
    ui->comboBoxPlatform->clear();
    ui->comboBoxPlatform->addItem("No Platform");
    ui->comboBoxPlatform->addItems(m_platforms);
    show();
}


void AddEmulatorDialog::on_buttonAdd_pressed()
{
    m_ok = true;
    this->hide();
}

void AddEmulatorDialog::on_buttonAddPlatform_pressed()
{
    QComboBox* newBox = new QComboBox(this);
    newBox->addItems(m_platforms);
    ui->layoutPlatforms->addWidget(newBox,m_nextPlatformRow,1);
    m_nextPlatformRow++;
    ui->layoutPlatforms->addWidget(ui->buttonAddPlatform,m_nextPlatformRow,0);
    m_newBoxes.append(newBox);
}

void AddEmulatorDialog::on_buttonCancel_pressed()
{
    m_ok = false;
    this->hide();
}

void AddEmulatorDialog::on_buttonExit_pressed()
{
    m_ok = false;
    this->hide();
}

void AddEmulatorDialog::on_buttonSearchPath_pressed()
{
    QFileDialog dialog;
    QString exec = QFileDialog::getOpenFileName(this, "Select game launcher file");
    qDebug(qUtf8Printable(exec));
    QString execName = exec.split("/").last();
    execName = execName.replace(".exe","");
    qDebug(qUtf8Printable(execName));
    if (!exec.isEmpty())
    {
        ui->lineEditPath->setText(exec);
        ui->lineEditName->setText(execName);
    }
}

Emulator* AddEmulatorDialog::getEmulator()
{
    if (m_ok)
    {
        QStringList platforms;
        QString platform_added("");
        platforms << ui->comboBoxPlatform->currentText();
        if(!(platforms.contains("No Platform"))){
            foreach (QComboBox* result, m_newBoxes)
            {
                platform_added = result->currentText();
                if (!platforms.contains(platform_added))
                {
                    platforms << platform_added;
                }
            }
        }
        else{
            qDebug() << "HELLO MOTOROLA";
            platforms = QStringList();
        }
        Emulator* emulatorResult = new Emulator(ui->lineEditPath->text(),ui->lineEditName->text(),platforms);
        return emulatorResult;
    }
    else
    {
        Emulator* emulatorEmpty = new Emulator("","",QStringList());
        return emulatorEmpty;
    }

}

