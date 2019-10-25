#include "settings.h"
#include "ui_settings.h"
#include "dbmanager.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->listWidget->addItem("General");
    ui->listWidget->addItem("Nothing yet");
    ui->listWidget->setCurrentRow(0);
    connect(ui->listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(tabChange()));

    if (!QFile::exists("settings.ini"))
    {
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("smoothScrolling", true);
        settings.setValue("deletePlatform", true);
        settings.setValue("newGameDesign", true);
        settings.setValue("emulatorsDir","/emulators");
    }

}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_buttonExit_clicked()
{
    this->setState(false);
    this->close();
}

void Settings::resizeWindow()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    this->resize(width*0.6,height*0.6);
}

void Settings::tabChange()
{
    ui->stackedWidget->setCurrentIndex(ui->listWidget->currentRow());
}

bool Settings::getState()
{
    return m_wasActive;
}

void Settings::setState(bool state)
{
    m_wasActive = state;
}

void Settings::on_checkBoxSmoothScrolling_toggled(bool state)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    qDebug(qUtf8Printable(QString::number(state)));
    if(state)
    {
        ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        ui->listWidget->verticalScrollBar()->setSingleStep(15);
    }
    else
    {
        ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    }
    settings.setValue("smoothScrolling",state);
    emit smoothScrolling(state);
}

void Settings::on_checkBoxGameDesign_toggled(bool state)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    setNewGameDesign(state);
    settings.setValue("newGameDesign",state);
    if (!infoPopup)
    {
        QMessageBox::information(this,"Settings","Changes will take effect on next application startup.");
        infoPopup = true;
    }
}

void Settings::on_checkBoxDeletePlatform_toggled(bool state)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    if(state)
    {
        //hmmmm delete here...
    }
    setting_deletePlatform = state;
    settings.setValue("deletePlatform",state);
}

bool Settings::getDeletePlatform()
{
    return setting_deletePlatform;
}


void Settings::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    setting_smoothScrolling = settings.value("smoothScrolling").toBool();
    ui->checkBoxSmoothScrolling->setChecked(setting_smoothScrolling);
    on_checkBoxSmoothScrolling_toggled(setting_smoothScrolling);

    setting_deletePlatform = settings.value("deletePlatform").toBool();
    ui->checkBoxDeletePlatform->setChecked(setting_deletePlatform);
    on_checkBoxDeletePlatform_toggled(setting_deletePlatform);

    setting_newGameDesign = settings.value("newGameDesign").toBool();
    ui->checkBoxGameDesign->setChecked(setting_newGameDesign);
    infoPopup = false;

    setting_emulatorsDir = settings.value("emulatorsDir").toString();
    ui->lineEditEmulatorsDir->setText(setting_emulatorsDir);
}


QString Settings::getEmulatorsDir()
{
    return setting_emulatorsDir;
}

void Settings::on_buttonEmulatorsDir_pressed()
{
    QFileDialog dialog;
    QString exec = dialog.getExistingDirectory(this,"Select emulators directory","",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (!exec.isEmpty())
    {
        ui->lineEditEmulatorsDir->setText(exec);
    }
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("emulatorsDir",ui->lineEditEmulatorsDir->text());
    setting_emulatorsDir = ui->lineEditEmulatorsDir->text();
}

bool Settings::getNewGameDesign()
{
    return setting_newGameDesign;
}

void Settings::setNewGameDesign(bool state)
{
    setting_newGameDesign = state;
}
