#include "listedpcgamedialog.h"
#include "ui_listedpcgamedialog.h"

ListedPCGameDialog::ListedPCGameDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListedPCGameDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
}

ListedPCGameDialog::~ListedPCGameDialog()
{
    delete ui;
}

void ListedPCGameDialog::setButtonEnabled()
{
    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonAdd->setText("Add selected games");
}

void ListedPCGameDialog::addGame(ListedPCGame* gameWidget)
{
    QListWidgetItem* game = new QListWidgetItem;
    QVariant v;
    QString gameinfo = gameWidget->getGameName()+"{}"+gameWidget->getPlatform()+"{}"+QString::number(gameWidget->getId())+"{}"+QString::number(gameWidget->getChecked())+"{}"+QString::number(gameWidget->getSteamId());
    v.setValue(gameinfo);
    game->setData(Qt::UserRole, v);
    game->setSizeHint(gameWidget->size());
    ui->listWidget->addItem(game);
    ui->listWidget->setItemWidget(game,gameWidget);
}

void ListedPCGameDialog::on_pushButtonAdd_pressed()
{
    qDebug(qUtf8Printable("gonnaemit"));
    QList<QString>* list = new QList<QString>;
    QVariant v;
    QString gameinfo;
    int index = ui->listWidget->count();
    for (int i=0;i<index;i++)
    {
        v = ui->listWidget->item(i)->data(Qt::UserRole);
        gameinfo = v.value<QString>();
        qDebug(qUtf8Printable("OH NO"));
        qDebug(qUtf8Printable(gameinfo));
        list->append(gameinfo);
    }
    qDebug(qUtf8Printable("hi1"));
    this->hide();
    emit askAddGames(list);

}

void ListedPCGameDialog::on_pushButtonCancel_pressed()
{
    this->hide();
}

void ListedPCGameDialog::clearContent()
{
    ui->listWidget->clear();
    ui->pushButtonAdd->setText("Wait...");
    ui->pushButtonAdd->setEnabled(false);
}
