#ifndef LISTEDPCGAMEDIALOG_H
#define LISTEDPCGAMEDIALOG_H

#include <QWidget>
#include <QListWidgetItem>
#include <listedpcgame.h>

namespace Ui {
class ListedPCGameDialog;
}

class ListedPCGameDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ListedPCGameDialog(QWidget *parent = 0);
    ~ListedPCGameDialog();
    void setButtonEnabled();
    void addGame(ListedPCGame* gameWidget);
    void clearContent();

signals:
    void askAddGames(QList<QString>* list);
private slots:
    void on_pushButtonAdd_pressed();
    void on_pushButtonCancel_pressed();
private:
    Ui::ListedPCGameDialog *ui;
};

#endif // LISTEDPCGAMEDIALOG_H
