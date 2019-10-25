#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    ui->listWidget->addItem("Getting started");
    ui->listWidget->addItem("About me");
    ui->listWidget->setCurrentRow(0);
    connect(ui->listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(tabChange()));
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("black"));
    effect->setOffset(1,1);
    ui->textBrowser->setGraphicsEffect(effect);
}

Help::~Help()
{
    delete ui;
}

void Help::on_buttonExit_clicked()
{
    this->setState(false);
    this->close();
}

void Help::resizeWindow()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    this->resize(width*0.4,height*0.4);
}

void Help::tabChange()
{
    ui->stackedWidget->setCurrentIndex(ui->listWidget->currentRow());
}

bool Help::getState()
{
    return m_wasActive;
}

void Help::setState(bool state)
{
    m_wasActive = state;
}
