#include "adddialog.h"

AddDialog::AddDialog()
{
    QLabel *ask = new QLabel("What do you want to add ?");
    QRadioButton *steam = new QRadioButton("Add detected installed PC games");
    QRadioButton *game = new QRadioButton("Add a game manually");
    QRadioButton *emulator = new QRadioButton("Add an emulator");
    QRadioButton *extsoft = new QRadioButton("Add an external software");
    QDialogButtonBox *ok = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(ask);
    layout->addWidget(steam);
    layout->addWidget(game);
    layout->addWidget(emulator);
    layout->addWidget(extsoft);
    layout->addWidget(ok);
    this->setLayout(layout);
    m_steam = steam;
    m_game = game;
    m_emulator = emulator;
    m_extsoft = extsoft;
    connect(ok,SIGNAL(accepted()),this,SLOT(getCheckedButton()));
    connect(ok,SIGNAL(rejected()),this,SLOT(close()));
    this->exec();
}

void AddDialog::getCheckedButton()
{
    if(m_steam->isChecked())
    {
        this->close();
        m_choice = 1;
    }
    else if (m_game->isChecked())
    {
        this->close();
        m_choice = 2;
    }
    else if (m_emulator->isChecked()) {
        this->close();
        m_choice = 3;
    }
    else if (m_extsoft->isChecked())
    {
        this->close();
        m_choice = 4;
    }
    else
    {
        this->close();
    }
}

int AddDialog::getChoice()
{
    return m_choice;
}
