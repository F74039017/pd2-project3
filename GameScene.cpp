#include "GameScene.h"
#include <QDebug>
#include <cstdlib>
#include <cstring>
#define tbName "rank"
#include "Game.h"
#include "Mainwindow.h"
#include <QGraphicsSceneMouseEvent>

GameScene::GameScene(QObject *parent)
    :QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QColor(Qt::black)));


    /* set game background */
    gameBG = new QGraphicsPixmapItem(QPixmap("")); //++
    gameBG->setScale(0.731);
    gameBG->setZValue(-1);
    addItem(gameBG);
    setSceneRect(0,0,1000,650);

    /* add line */
    QPen pen = QPen(QColor(Qt::white));
    for(int i=0; i<hnum+1; i++) // horizon
        addLine(125, 125+50*i, 875, 125+50*i, pen);
    for(int i=0; i<wnum+1; i++)
        addLine(125+50*i, 125, 125+50*i, 625, pen);

    /* add squares */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            squares[i][j] = new Square();
            squares[i][j]->setScale(0.1);
            squares[i][j]->setcor(i, j);
            squares[i][j]->setPos(125+50*j,125+50*i);
            squaresRect[i][j].setRect(125+50*j, 125+50*i, 50, 50);
            addItem(squares[i][j]);
            squares[i][j]->hide();
        }


    /* add title */
    title = new QGraphicsPixmapItem(QPixmap(":/images/images/2048_title_in.png"));
    title->setPos(100, -10);
    title->setScale(0.35);
    addItem(title);

    /* Score */
    scoreLabel = new QGraphicsSimpleTextItem("Score:");
    scoreLabel->setPos(600, 35);
//    scoreLabel->setBrush(QBrush(QColor(220, 70, 70)));
    scoreLabel->setBrush(QBrush(QColor(Qt::white)));
    QFont scoreLabelFont("URW Chancery L", 25);
    scoreLabelFont.setItalic(true);
    scoreLabelFont.setUnderline(true);
    scoreLabel->setFont(scoreLabelFont);
    addItem(scoreLabel);
    score = new QGraphicsSimpleTextItem("0");
    score->setPos(700, 75);
//    score->setBrush(QBrush(QColor(220, 70, 70)));
    score->setBrush(QBrush(QColor(Qt::white)));
    QFont scoreFont("URW Chancery L", 25);
    scoreFont.setItalic(true);
    score->setFont(scoreFont);
    addItem(score);

    /* GameOver */
    gameoverBG = new QGraphicsRectItem(0, 0, 500, 650);
    gameoverBG->setBrush(QBrush(QColor(128, 128, 128)));
    gameoverBG->setOpacity(0.85);
    gameoverBG->setPen(Qt::NoPen);
    addItem(gameoverBG);
    gameoverLabel = new QGraphicsSimpleTextItem("Game Over");
    gameoverLabel->setPos(55, 80);
    gameoverLabel->setBrush(QColor(Qt::black));
    QPen gameoverPen(QColor(Qt::white));
    gameoverPen.setWidth(3);
    gameoverLabel->setPen(gameoverPen);
    QFont gameoverFont("URW Chancery L", 55);
    gameoverFont.setItalic(true);
    gameoverFont.setBold(true);
    gameoverFont.setLetterSpacing(QFont::AbsoluteSpacing, 10);
    gameoverLabel->setFont(gameoverFont);
    addItem(gameoverLabel);

    /* Gameover Score */
    gameoverScoreLabel = new QGraphicsSimpleTextItem("Your Score:");
    gameoverScoreLabel->setPos(85, 250);
    gameoverScoreLabel->setBrush(QColor(Qt::black));
    gameoverFont.setPointSize(32);
    gameoverFont.setBold(true);
    gameoverFont.setLetterSpacing(QFont::AbsoluteSpacing, 3);
    gameoverPen.setWidth(1);
    gameoverScoreLabel->setPen(gameoverPen);
    gameoverScoreLabel->setFont(gameoverFont);
    addItem(gameoverScoreLabel);
    bestScoreLabel = new QGraphicsSimpleTextItem("Your Best:");
    bestScoreLabel->setPos(85, 350);
    bestScoreLabel->setBrush(QColor(Qt::black));
    bestScoreLabel->setPen(gameoverPen);
    bestScoreLabel->setFont(gameoverFont);
    addItem(bestScoreLabel);
    gameoverScore = new QGraphicsSimpleTextItem("0");
    gameoverScore->setPos(310, 250);
    gameoverScore->setBrush(QColor(Qt::black));
    gameoverScore->setPen(gameoverPen);
    gameoverScore->setFont(gameoverFont);
    addItem(gameoverScore);
    bestScore = new QGraphicsSimpleTextItem("0");
    bestScore->setPos(310, 350);
    bestScore->setBrush(QColor(Qt::black));
    bestScore->setPen(gameoverPen);
    bestScore->setFont(gameoverFont);
    addItem(bestScore);

    /* again icon */
    againIcon = new Icon(Icon::AGAIN);
    againIcon->setScale(0.28);
    againIcon->setPos(40, 480);
    addItem(againIcon);
    againIcon->hide();

    /* back icon */
    backIcon = new Icon(Icon::BACK);
    backIcon->setScale(0.28);
    backIcon->setPos(300, 480);
    addItem(backIcon);

    /* rect init */
    int w, h;
    w = againIcon->boundingRect().width()*0.28;
    h = againIcon->boundingRect().height()*0.28;
    againIconRect = new QRect(40, 480, w, h);
    backIconRect = new QRect(300, 480, w, h);

    /* init group */
    linkgroup = new QParallelAnimationGroup(this);
    fallSquenceGroup = new QSequentialAnimationGroup(this);
    exchangeGroup = new QParallelAnimationGroup(this);

    /* time (survive) mdoe */
    timer = new QTimer(this);
    timeLabel = new QGraphicsSimpleTextItem("15");
    timeLabel->setPos(400, 20);
    timeLabel->setBrush(QBrush(QColor(Qt::yellow)));
    QFont timeLabelFont("URW Chancery L", 25);
    timeLabelFont.setItalic(true);
    timeLabel->setFont(timeLabelFont);
    addItem(timeLabel);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(countDown()));

    /* connect animation */
    QObject::connect(linkgroup, SIGNAL(finished()), this, SLOT(endDisappearAnimation()));
    QObject::connect(fallSquenceGroup, SIGNAL(finished()), this, SLOT(endFallAnimation()));
    QObject::connect(exchangeGroup, SIGNAL(finished()), this, SLOT(endExchangeAnimation()));

}

void GameScene::init()
{
    /* get number of rank */
    QSqlQuery qry;
    qry.exec(QString("SELECT COUNT(*) FROM %1").arg(tbName));
    qry.next();
//    qDebug() << "row now is " << qry.value(0).toString();
    recordNum = qry.value(0).toInt();
//    qDebug() << "update new recordNum = " << recordNum;


    /* init squares */
    for(int i=0; i<hnum; i++)  // temp to show all squares in the window
        for(int j=0; j<wnum; j++)  //-- 0 and false after test
        {
            int type = rand() % Square::typenum;
            squares[i][j]->setType(static_cast<Square::Type>(type));
            squares[i][j]->setExist(true);
        }

    /* init score */
    score->setText("0");

    /* isAnimation */
    isAnimation = false;

    /* init recover point of squares */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            squares[i][j]->setRecoverPoint();

    /* init gameover */
    resetIcon();
    theEnd = false;
    isWin = false;
    gameoverBG->hide();
    gameoverBG->setZValue(1);
    gameoverLabel->hide();
    gameoverLabel->setZValue(1);
    gameoverScoreLabel->hide();
    gameoverScoreLabel->setZValue(1);
    bestScoreLabel->hide();
    bestScoreLabel->setZValue(1);
    gameoverScore->hide();
    gameoverScore->setZValue(1);
    bestScore->hide();
    bestScore->setZValue(1);
    againIcon->hide();
    againIcon->setZValue(1);
    backIcon->hide();
    backIcon->setZValue(1);
    backIcon->setSoundFlag(false);
    againIcon->setSoundFlag(false);

    /* get userName */
    userName = dynamic_cast<Game*>(parent())->getuserName();

    /* init islink */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            islink[i][j] = false;

    /* init clickCount */
    hasClick = false;

    /* init last index */
    lasti = lastj = 0;

    /* prevent animation disappear broken */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            squares[i][j]->setOpacity(1);

    timeLabel->hide();

    /* init squares */
    initsquares();

}

void GameScene::resetIcon()
{
    backIcon->setImage(backIcon->getType());
    againIcon->setImage(againIcon->getType());
}

void GameScene::initsquares()
{
    while(set3Link())
    {
        for(int i=0; i<hnum; i++)
            for(int j=0; j<wnum; j++)
            {
                if(islink[i][j])
                {
                    int type = rand()%Square::typenum;
                    squares[i][j]->setType(static_cast<Square::Type>(type));
                }
                islink[i][j] = false;
            }
    }
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isAnimation)
        return;

    if(backIconRect->contains(event->scenePos().toPoint()) && theEnd)
    {
        if(!Mainwindow::soundMute)
            backIcon->playClickSound();
        if(isWin)
            insertRank();
        emit pressBack();
    }


    exchangeGroup->clear();
    /* find clicked squares */
    bool find = false;
    bool valid = false;
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum && !find; j++)
        {
            if(squaresRect[i][j].contains(event->scenePos().toPoint()))
            {
                find = true;
                if(!hasClick)   // record last position
                {
                    lasti = i;
                    lastj = j;
                }
                else
                {
                    /* check valid */
                    if((i==lasti+1&&j==lastj)||(i==lasti-1&&j==lastj)||(i==lasti&&j==lastj+1)||(i==lasti&&j==lastj-1))
                        valid = true;
                    if(!valid)
                        break;

                    /* set curi */
                    curi = i;
                    curj = j;

                    /* exchange */
                    Square::Type temp = squares[i][j]->getType();
                    *squares[i][j] = *squares[lasti][lastj];
                    squares[lasti][lastj]->setType(temp);
                    squares[i][j]->setMoveStart(squares[lasti][lastj]->pos());
                    squares[lasti][lastj]->setMoveStart(squares[i][j]->pos());
                    exchangeGroup->addAnimation(squares[i][j]->getMoveAnimation());
                    exchangeGroup->addAnimation(squares[lasti][lastj]->getMoveAnimation());
                }
                break;
            }
        }
    if(!hasClick)
        hasClick = true;
    else
    {
        hasClick = false;
        if(valid)
        {
            isReExchange = false;
            exchangeGroup->start();
        }
    }

}

void GameScene::keyPressEvent(QKeyEvent *event)
{
//    Fall();
//    for(int i=0; i<hnum; i++)
//        for(int j=0; j<wnum; j++)
//            islink[i][j] = false;
}

int GameScene::set3Link()
{
    int linknumber=0;
    /* scan three link */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            if(!(0>i-1 || i+1==hnum) && *squares[i][j]==*squares[i+1][j] && *squares[i][j]==*squares[i-1][j])
                islink[i][j] = islink[i+1][j] = islink[i-1][j] = true, linknumber++;
            if(!(0>j-1 || j+1==wnum) && *squares[i][j]==*squares[i][j+1] && *squares[i][j]==*squares[i][j-1])
                islink[i][j] = islink[i][j+1] = islink[i][j-1] = true, linknumber++;
        }
    return linknumber;
}

void GameScene::Fall()
{
    /* show all squares */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            squares[i][j]->setOpacity(1);

    /* Create new animation */
    fallgroup = new QParallelAnimationGroup(this);
    fallgroup2 = new QParallelAnimationGroup(this);
    QObject::connect(fallgroup, SIGNAL(finished()), this, SLOT(reshow()));

    /* calculate destination */
    int k;
    for(int j=0; j<wnum; j++)
    {
        k = hnum-1;
        for(int i=hnum-1; i>=0; i--)
            if(!islink[i][j])
            {
                if(i!=k)    // animation
                {
                    *squares[k][j] = *squares[i][j];  // fall and pass type
                    squares[k][j]->setMoveStart(squares[i][j]->pos());
                    fallgroup->addAnimation(squares[k][j]->getMoveAnimation());
                }
                k--;
            }
        for(; k>=0; k--)
        {
            int type = rand()%Square::typenum;
            squares[k][j]->setType(static_cast<Square::Type>(type));
            squares[k][j]->setMoveStart(QPointF(squares[k][j]->x(), squares[k][j]->y()-50));
            fallgroup2->addAnimation(squares[k][j]->getMoveAnimation());
            squares[k][j]->hide();
        }
    }
    fallSquenceGroup->addAnimation(fallgroup);
    fallSquenceGroup->addAnimation(fallgroup2);

    fallSquenceGroup->start();
}

/* recover special position */
void GameScene::reservedSquares()
{
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            if(reservedlink[i][j])
                islink[i][j] = false;
}

/* check x squares in a line and set islink[] */
void GameScene::checkX(int x)
{
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            if(!islink[i][j])
            {
                dfs(i, j, x, 0, 1, squares[i][j]->getType());   // horizon
                dfs(i, j, x, 0, 2, squares[i][j]->getType());   // vertical
            }
}

bool GameScene::dfs(int i, int j, int x, int cnt, int dir, Square::Type lastType)
{
    if(i>=hnum || j>=wnum || lastType!=squares[i][j]->getType() || islink[i][j])  // used or out of border
    {
        if(cnt>=x)
            return true;
        else
            return false;
    }

    if(dir==1)  // right
    {
        if(dfs(i, j+1, x, cnt+1, dir, squares[i][j]->getType()))
        {
            islink[i][j] = true;
            if(cnt==0)
                squares[i][j]->setType(Square::NON), reservedlink[i][j] = true;
            return true;
        }
        else
            return false;
    }
    else    // down
    {
        if(dfs(i+1, j, x, cnt+1, dir, squares[i][j]->getType()))
        {
            islink[i][j] = true;
            if(cnt==0)
                squares[i][j]->setType(Square::NON), reservedlink[i][j] = true;
            return true;
        }
        else
            return false;
    }
}



int GameScene::checkend()
{

}

void GameScene::gameover()
{

}

void GameScene::addScore()
{

}

void GameScene::startLinkAnimation()
{
    isAnimation = true;
    linkgroup->start();
}

void GameScene::startFallAnimation()
{
    fallgroup->start();
}

void GameScene::insertRank()
{
    QSqlQuery qry;
    if(!qry.exec(QString("INSERT INTO %1 VALUES (%2, '%3', %4)").arg(tbName, QString::number(recordNum), userName, score->text())));
        qDebug() << qry.lastError().text();
}

void GameScene::endDisappearAnimation()
{
    linkgroup->clear();
    Fall();
}

void GameScene::endFallAnimation()
{
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            islink[i][j] = false;
    fallSquenceGroup->clear();

    set3Link();  // bool flag
    int anima_cnt=0;
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            if(islink[i][j])
            {
                squares[i][j]->setDisapper();
                linkgroup->addAnimation(squares[i][j]->getDisappearAnimation());
                anima_cnt++;
            }
        }
    if(!anima_cnt)
    {
        isAnimation = false;
        return;
    }
    startLinkAnimation();
}

void GameScene::reshow()
{
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            squares[i][j]->show();
}

void GameScene::endExchangeAnimation()
{
    if(isReExchange)
    {
        exchangeGroup->clear();
        return;
    }

    /* reset reservedlink before a move */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            reservedlink[i][j] = false;

    /* check 5 squares in line, and create special squares */
    checkX(5);  // check and set reservedlink

    /* check 4 squares in line */
    checkX(4);

    /* disappear animation */
    if(set3Link())  // bool flag
    {
        reservedSquares();  // protect special position
        for(int i=0; i<hnum; i++)
            for(int j=0; j<wnum; j++)
                if(islink[i][j])
                {
                    squares[i][j]->setDisapper();
                    linkgroup->addAnimation(squares[i][j]->getDisappearAnimation());
                }
        startLinkAnimation();
    }
    else    // re-exchange
    {
        isReExchange = true;
        Square::Type temp = squares[curi][curj]->getType();
        *squares[curi][curj] = *squares[lasti][lastj];
        squares[lasti][lastj]->setType(temp);
        exchangeGroup->start();
    }
}

//void GameScene::countDown()
//{
//    int nextTime = timeLabel->text().toInt()-1;
//    if(nextTime == -1)
//        gameover();
//    else
//        timeLabel->setText(QString::number(nextTime));
//}
