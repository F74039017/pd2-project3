#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QColor>
#include <QKeyEvent>
#include "Square.h"
#include <QtSql>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include "IconAgain.h"
#include "IconBack.h"
#include <QTimer>
#include <QPair>
#include <QVector>
using namespace std;
#define wnum 15 // # of squares
#define hnum 10

using namespace std;

class GameScene: public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode{STEP, TIME};
    GameScene(QObject *parent=0);
    ~GameScene();
    void initsquares();
    void setMode(GameScene::Mode mode);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void init();
    bool getisAnimation();

signals:
    void pressBack();
    void quit(int star, int score);

public slots:
    void endDisappearAnimation();
    void endFallAnimation();
    void reshow();
    void endExchangeAnimation();
    void countDown();

private:
    GameScene::Mode mode;
    QGraphicsPixmapItem *gameBG;
    QGraphicsPixmapItem *title;
    QGraphicsSimpleTextItem *scoreLabel;
    QGraphicsSimpleTextItem *score;
    QGraphicsRectItem *gameoverBG;
    QGraphicsSimpleTextItem *gameoverLabel;
    QGraphicsSimpleTextItem *gameoverScoreLabel;
    QGraphicsSimpleTextItem *gameoverScore;
    QGraphicsSimpleTextItem *bestScoreLabel;
    QGraphicsSimpleTextItem *bestScore;
    Square *squares[hnum][wnum];
    bool islink[hnum][wnum];
    QRect squaresRect[hnum][wnum];
    bool theEnd;
    bool isAnimation;
    Icon *backIcon;
    Icon *againIcon;
    QRect *againIconRect;
    QRect *backIconRect;
    QParallelAnimationGroup *linkgroup;
    QParallelAnimationGroup *fallgroup;
    QParallelAnimationGroup *fallgroup2;
    QSequentialAnimationGroup *fallSquenceGroup;
    QParallelAnimationGroup *exchangeGroup;
    QParallelAnimationGroup *reexchangeGroup;
    bool isReExchange;
    QTimer *timer;
    QGraphicsSimpleTextItem *limitLabel;
    QString userName;
    int recordNum;
    bool hasClick;
    int lasti, lastj;
    int curi, curj;
    bool specialLink;
    int sposi, sposj;
    int newspecial[hnum][wnum];
    int addvalue;
    QGraphicsPixmapItem *star[3];
    QGraphicsPixmapItem *finalstar[3];
    int starNum;

    bool checkend();
    void gameover();
    void addScore();
    void startLinkAnimation();
    void startFallAnimation();
    void insertRank();
    bool set3Link();
    bool checkL();
    void doEffect(bool recur_star = true);
    void Fall();
    bool checkX(int x);
    bool dfs(int i, int j, int x, int cnt, int dir, Square::Type lastType);
    bool setLink();
    void setSpecial();
    void updateStar();
    void polymorphismSetIconPixmap();
};

#endif // GAMESCENE_H
