#ifndef SEARCHGAME_H
#define SEARCHGAME_H

#include <QWidget>

namespace Ui {
class SearchGame;
}

class SearchGame : public QWidget
{
    Q_OBJECT

public:
    explicit SearchGame();
    explicit SearchGame(QWidget *parent, QString gameName="", QString platform="", QString releaseDate="", int id=-1);
    ~SearchGame();
    void setGame(QString gameName="", QString platform="", QString releaseDate="", int id=-1);
    QString getGameName();
    QString getPlatform();
    int getGameId();


private slots:
    void on_buttonAdd_clicked();

signals:
    void askAddGame(SearchGame* that);


private:


    Ui::SearchGame *ui;
    QString m_gameName;
    QString m_platform;
    QString m_releaseDate;
    int m_id;

};


#endif // SEARCHGAME_H
