#include "addexternalsoftwaredialog.h"
#include "ui_addexternalsoftwaredialog.h"

AddExternalSoftwareDialog::AddExternalSoftwareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddExternalSoftwareDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    DbManager db("data.db");
    QStringList platforms = db.getAllPlatforms();
    m_platforms = platforms;
    ui->comboBoxPlatform->addItems(m_platforms);
}

AddExternalSoftwareDialog::~AddExternalSoftwareDialog()
{
    delete ui;
}

void AddExternalSoftwareDialog::showUp(){

    DbManager db("data.db");
    m_platforms = db.getAllPlatforms();
    ui->comboBoxPlatform->clear();
    ui->comboBoxPlatform->addItem("No Platform");
    ui->comboBoxPlatform->addItem("All Platforms");
    ui->comboBoxPlatform->addItems(m_platforms);
    show();
}

void AddExternalSoftwareDialog::on_buttonAdd_pressed()
{
    m_ok = true;
    updateExternalSoftware();
    emit acceptedSoftware(m_extSoftName,m_extSoftPath,m_platformsAccepted);
    clear();
    this->hide();
}

void AddExternalSoftwareDialog::on_buttonAddPlatform_pressed()
{
    QComboBox* newBox = new QComboBox(this);
    newBox->addItems(m_platforms);
    ui->layoutPlatforms->addWidget(newBox,m_nextPlatformRow,1);
    m_nextPlatformRow++;
    ui->layoutPlatforms->addWidget(ui->buttonAddPlatform,m_nextPlatformRow,0);
    m_newBoxes.append(newBox);
}


void AddExternalSoftwareDialog::on_buttonCancel_pressed()
{
    m_ok = false;
    clear();
    this->hide();
}

void AddExternalSoftwareDialog::on_buttonExit_pressed()
{
    m_ok = false;
    clear();
    this->hide();
}

void AddExternalSoftwareDialog::on_buttonSearchPath_pressed()
{
    QFileDialog dialog;
    QString exec = QFileDialog::getOpenFileName(this, "Select software file");
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

void AddExternalSoftwareDialog::clear(){

    ui->lineEditName->setText("");
    ui->lineEditPath->setText("");
    for(int i=0;i<m_newBoxes.length();i++)
    {
        QComboBox* cb = m_newBoxes.at(0);
        m_newBoxes.removeAt(0);
        cb->deleteLater();
    }
}


void AddExternalSoftwareDialog::updateExternalSoftware()
{
    if (m_ok)
    {
        QStringList platforms;
        QString platform_added("");
        platforms << ui->comboBoxPlatform->currentText();
        if(!platforms.contains("No Platform")){
            if(!platforms.contains("All Platforms")){
                foreach (QComboBox* result, m_newBoxes)
                {
                    platform_added = result->currentText();
                    if ((!platforms.contains(platform_added))&&(platform_added!="No Platform")&&(platform_added!="All Platforms"))
                    {
                        platforms << platform_added;
                    }
                }
            }
            else{
                platforms = m_platforms;
            }
        }
        else{
            platforms = QStringList();
        }
        m_extSoftName = ui->lineEditName->text();
        m_extSoftPath = ui->lineEditPath->text();
        m_platformsAccepted = platforms;
    }
}
