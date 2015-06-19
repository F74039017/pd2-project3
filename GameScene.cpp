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
//            squares[i][j]->setcor(i, j);
            squares[i][j]->setPos(125+50*j,125+50*i);
            squaresRect[i][j].setRect(125+50*j, 125+50*i, 50, 50);
            addItem(squares[i][j]);
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
    initsquares();

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

    /* init special link */
    specialLink = false;

    /* test L check --*/
//    squares[1][2]->setType(Square::FIRE);
//    squares[1][3]->setType(Square::FIRE);
//    squares[2][1]->setType(Square::FIRE);
//    squares[3][1]->setType(Square::FIRE);
//    squares[1][1]->setType(Square::WATER);
//    squares[1][0]->setType(Square::FIRE);

    /* test 4 in line */
//    squares[0][1]->setType(Square::FIRE);
//    squares[1][1]->setType(Square::FIRE);
//    squares[2][1]->setType(Square::FIRE);
//    squares[4][1]->setType(Square::FIRE);
//    squares[3][1]->setType(Square::WATER);
//    squares[3][2]->setType(Square::FIRE);
//    squares[5][1]->setType(Square::FIRE);

    /* test L */
//    squares[0][1]->setType(Square::FIRE);
//    squares[1][1]->setType(Square::FIRE);
//    squares[2][1]->setType(Square::WATER);
//    squares[3][1]->setType(Square::FIRE);
//    squares[2][2]->setType(Square::FIRE);
//    squares[2][3]->setType(Square::FIRE);

    /* test star */
    squares[0][1]->setType(Square::FIRE);
    squares[1][1]->setType(Square::FIRE);
    squares[2][1]->setType(Square::WATER);
    squares[2][2]->setType(Square::FIRE);
    squares[3][1]->setType(Square::FIRE);
    squares[4][1]->setType(Square::FIRE);
}

void GameScene::resetIcon()
{
    backIcon->setImage(backIcon->getType());
    againIcon->setImage(againIcon->getType());
}

void GameScene::initsquares()
{
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            squares[i][j]->randCreate();

    while(set3Link())
    {
        for(int i=0; i<hnum; i++)
            for(int j=0; j<wnum; j++)
            {
                if(islink[i][j])
                {
                    squares[i][j]->randCreate();
                }
                islink[i][j] = false;
            }
    }
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(backIconRect->contains(event->scenePos().toPoint()) && theEnd)
    {
        if(!Mainwindow::soundMute)
            backIcon->playClickSound();
        if(isWin)
            insertRank();
        emit pressBack();
    }

    if(isAnimation)
        return;

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
                    Square temp;
                    temp = *squares[i][j];
                    *squares[i][j] = *squares[lasti][lastj];
                    *squares[lasti][lastj] = temp;
                    squares[i][j]->setMoveStart(squares[lasti][lastj]->pos());
                    squares[lasti][lastj]->setMoveStart(squares[i][j]->pos());
                    exchangeGroup->addAnimation(squares[i][j]->getMoveAnimation());
                    exchangeGroup->addAnimation(squares[lasti][lastj]->getMoveAnimation());

                    /* set bomb and star link */
                    if(squares[curi][curj]->getType()==Square::BOMB || squares[curi][curj]->getType()==Square::STAR)
                        islink[curi][curj] = true, specialLink = true;
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
            isAnimation = true;
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
    for(int i=0; i<hnum; i++)
        qDebug() << "i=" << i << " " << squares[i][1]->getType();
}

bool GameScene::set3Link()
{
    bool flag = false;
    /* scan three link */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            /* 3 squares in line */
            if(!(0>i-1 || i+1==hnum) && *squares[i][j]==*squares[i+1][j] && *squares[i][j]==*squares[i-1][j])
                islink[i][j] = islink[i+1][j] = islink[i-1][j] = true, flag = true;
            if(!(0>j-1 || j+1==wnum) && *squares[i][j]==*squares[i][j+1] && *squares[i][j]==*squares[i][j-1])
                islink[i][j] = islink[i][j+1] = islink[i][j-1] = true, flag = true;
        }
    return flag;
}

bool GameScene::checkL()
{
    bool flag = false;
    const int mi[4] = {-1, 0, 1, 0};    // up right down left
    const int mj[4] = {0, 1, 0, -1};
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            QVector<QPair<int, int> > record[4];
            for(int k=0; k<4; k++)  // expand and track
            {
                int e=1;
                while((0<=i+mi[k]*e) && (i+mi[k]*e<hnum) && (0<=j+mj[k]*e) && (j+mj[k]*e<wnum)) // in range
                {
                    if(*squares[i+mi[k]*e][j+mj[k]*e] != *squares[i][j])
                        break;

                    /* record pos in vector */
                    record[k].push_back(qMakePair(i+mi[k]*e, j+mj[k]*e));
                    e++;
                }
            }

            /* find L */
            bool valid = false;
            for(int k=0; k<4; k++)
                if(record[k].size()>=2 && record[(k+1)%4].size()>=2)
                    valid = true;

            /* set special squares */
            if(valid)
            {
                squares[i][j]->setType(Square::BOMB), reservedlink[i][j] = true;

                /* set islink[] */
                for(int k=0; k<4; k++)
                    for(QVector<QPair<int, int> >::iterator iter=record[k].begin(); iter<record[k].end(); iter++)
                        islink[iter->first][iter->second] = true;

                /* set flag */
                flag = true;
            }
        }
    return flag;
}

void GameScene::doEffect()  // recursive effect
{
    bool flag = false;
    const int mi[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int mj[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            if(islink[i][j])    // disappear
            {
                Square::Type type = squares[i][j]->getType();
                switch(type)
                {
                case Square::BOMB:
                    squares[i][j]->setEffect(Square::NO_EFFECT);
                    for(int k=0; k<8; k++)
                        if(0<=i+mi[k] && i+mi[k]<hnum && 0<=j+mj[k] && j+mj[k]<wnum)    // in range
                            if(!islink[i+mi[k]][j+mj[k]])
                                islink[i+mi[k]][j+mj[k]] = true, flag = true;   // check islink[] expand
                    break;
                case Square::STAR:
                    squares[i][j]->setEffect(Square::NO_EFFECT);
                    for(int a=0; a<hnum; a++)
                        for(int b=0; b<wnum; b++)
                            if(*squares[lasti][lastj] == *squares[a][b])
                                islink[a][b] = true;
                    break;
                }
                Square::Effect effect = squares[i][j]->getEffect();
                switch (effect)
                {
                case Square::VERTICAL:
                    squares[i][j]->setEffect(Square::NO_EFFECT);
                    for(int k=0; k<hnum; k++)
                        if(!islink[k][j])
                            islink[k][j] = true, flag = true;
                    break;
                case Square::HORIZON:
                    squares[i][j]->setEffect(Square::NO_EFFECT);
                    for(int k=0; k<wnum; k++)
                        if(!islink[i][k])
                            islink[i][k] = true, flag = true;
                    break;
                }

            }
        }
    if(flag)    // call recursive
        doEffect();
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
            squares[k][j]->randCreate();
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

/* called by checkX to set reservedlink[] and assign new special squares */
bool GameScene::dfs(int i, int j, int x, int cnt, int dir, Square::Type lastType)
{
    if(i>=hnum || j>=wnum || lastType!=squares[i][j]->getType() || islink[i][j])  // used or out of border
    {
        if(cnt>=x)
        {
            islink[i][j] = true;
            return true;
        }
        else
            return false;
    }

    if(dir==1)  // horizon
    {
        if(dfs(i, j+1, x, cnt+1, dir, squares[i][j]->getType()))
        {
            islink[i][j] = true;
            if(cnt==0)
            {
                if(x == 4)
                    squares[i][j]->setEffect(Square::HORIZON);
                else if(x == 5)
                    squares[i][j]->setType(Square::STAR);
                reservedlink[i][j] = true;
            }
            return true;
        }
        else
            return false;
    }
    else    // vertical
    {
        if(dfs(i+1, j, x, cnt+1, dir, squares[i][j]->getType()))
        {
            islink[i][j] = true;
            if(cnt==0)
            {
                if(x == 4)
                    squares[i][j]->setEffect(Square::VERTICAL);
                else if(x == 5)
                    squares[i][j]->setType(Square::STAR);
                reservedlink[i][j] = true;
            }
            return true;
        }
        else
            return false;
    }
}

bool GameScene::setLink()
{
    bool flag = false;
    /* reset reservedlink before a move */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            reservedlink[i][j] = false;

    /* check 5 squares in line, and create special squares */
    checkX(5);  // check and set reservedlink

    /* check L link */
    if(checkL())
        flag = true;

    /* check 4 squares in line */
    checkX(4);

    /* set 3 squares in line */
    if(set3Link())
        flag = true;

    /* provent new special squares from disappeared */
    reservedSquares();

    /* effect chain */
    doEffect();

    return flag || specialLink; // specialLink check for bomb or star effects
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
    /* reset islink */
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
            islink[i][j] = false;
    fallSquenceGroup->clear();

    int hasLink = setLink();  // bool flag
    for(int i=0; i<hnum; i++)
        for(int j=0; j<wnum; j++)
        {
            if(islink[i][j])
            {
                squares[i][j]->setDisapper();
                linkgroup->addAnimation(squares[i][j]->getDisappearAnimation());
            }
        }
    if(!hasLink)
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
        isAnimation = false;
        return;
    }

    /* disappear animation */
    if(setLink())  // bool flag
    {
        specialLink = false;
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
        Square temp;
        temp = *squares[curi][curj];
        *squares[curi][curj] = *squares[lasti][lastj];
        *squares[lasti][lastj] = temp;
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
