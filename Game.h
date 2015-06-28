#ifndef GAME
#define GAME

#include <QGraphicsView>
#include "GameScene.h"
#include "IndexScene.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "GameScene.h"

class Game: public QGraphicsView{
    Q_OBJECT

public:
    Game(QWidget * parent = 0);
    ~Game();
    void init();    // restart
    void setUserName(QString name);
    QString getuserName();
    static int getlastScore();
    static int getlastStarNum();

signals:
    void toGame();
    void toIndex();

public slots:
    void setLastRecord(int star, int score);
    void restart();
    void toGameScene();
    void toIndexScene();
    void muteMusic();
    void setStepMode();
    void setTimeMode();

private:
    QSqlDatabase db;
    GameScene* gameScene;
    IndexScene *indexScene;
    QString userName;
    QMediaPlayer *bgm;
    QMediaPlaylist *playList;
    static int lastScore;
    static int lastStarNum;
};

#endif // GAME

