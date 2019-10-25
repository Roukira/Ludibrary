#include "listemulatorsdialog.h"
#include "ui_listemulatorsdialog.h"

ListEmulatorsDialog::ListEmulatorsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListEmulatorsDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

ListEmulatorsDialog::~ListEmulatorsDialog()
{
    delete ui;
}


void ListEmulatorsDialog::on_pushButtonAdd_pressed()
{
    DbManager db("data.db");
    QVariant v;
    QStringList platformList;
    QString emulatorInfo,name,platforms,path;
    int index = ui->listWidget->count();
    for (int i=0;i<index;i++)
    {
        v = ui->listWidget->item(i)->data(Qt::UserRole);
        emulatorInfo = v.value<QString>();
        name = emulatorInfo.split("{}").at(0);
        path = emulatorInfo.split("{}").at(2);
        platforms = emulatorInfo.split("{}").at(1);
        platformList = platforms.split(" and ");
        db.addEmulatorEntry(name.trimmed(),path);
        foreach (QString platform, platformList)
        {
            db.setPlatformEmulator(platform,name.trimmed());
        }
        emit newEmulator();
    }
    this->hide();
}

void ListEmulatorsDialog::on_pushButtonCancel_pressed()
{
    this->hide();
}

void ListEmulatorsDialog::on_buttonAdd_pressed()
{
    DbManager db("data.db");
    AddEmulatorDialog dialog;
    Emulator* emulator = dialog.getEmulator();
    emulator->hide_image();
    emulator->hide_edit();
    QListWidgetItem* emuWidget = new QListWidgetItem;
    QVariant v;
    QStringList platformList = emulator->getPlatforms();
    QString platforms = platformList.join(" and ");
    QString emulatorInfo = emulator->getName()+"{}"+platforms+"{}"+emulator->getPath();
    v.setValue(emulatorInfo);
    emuWidget->setData(Qt::UserRole, v);
    emuWidget->setSizeHint(emulator->size());
    ui->listWidget->addItem(emuWidget);
    ui->listWidget->setItemWidget(emuWidget,emulator);

}


void ListEmulatorsDialog::on_buttonDelete_pressed()
{
    int itemNumber(ui->listWidget->currentRow());
    if(itemNumber>-1)
    {
        delete ui->listWidget->currentItem();
    }
}

void ListEmulatorsDialog::clearContent()
{
    ui->listWidget->clear();
}

void ListEmulatorsDialog::on_buttonExit_pressed()
{
    this->hide();
}
