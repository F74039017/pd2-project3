#include "IndexScene.h"
#define tbName "rank"
#include "Game.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "Mainwindow.h"

IndexScene::IndexScene(QObject *parent)
    :QGraphicsScene(parent)
{
    indexBG = new QGraphicsPixmapItem(QPixmap(":/images/images/index_BG.png"));
    indexBG->setZValue(-1);
    addItem(indexBG);
    setSceneRect(0,0,1000,650);

    /* start icon */
    startIcon = new IconStart(Icon::START);
    startIcon->setScale(0.3);
    startIcon->setPos(200, 400);
    addItem(startIcon);
    startIcon->setSoundFlag(true);

    /* rank icon */
    rankIcon = new IconRank(Icon::RANK);
    rankIcon->setScale(0.3);
    rankIcon->setPos(650, 400);
    addItem(rankIcon);
    rankIcon->setSoundFlag(true);

    /* add backIcon */
    backIcon = new IconBack(Icon::BACK);
    backIcon->setScale(0.23);
    backIcon->setPos(550, 550);
    backIcon->setZValue(2);
    addItem(backIcon);
    backIcon->hide();
    backIcon->setSoundFlag(false);

    /* set icons images */
    polymorphismSetIconPixmap();

    /* rect init */
    int w, h;
    w = startIcon->boundingRect().width()*0.3;
    h = startIcon->boundingRect().height()*0.3;
    startIconRect = new QRect(200, 400, w, h);
    rankIconRect = new QRect(650, 400, w, h);
    w = startIcon->boundingRect().width()*0.23;
    h = startIcon->boundingRect().height()*0.23;
    backIconRect = new QRect(550, 550, w, h);
//    qDebug() << *startIconRect << " " << *rankIconRect;

    /* add rankBG */
    rankBG = new QGraphicsPixmapItem(QPixmap(":/images/images/rank_border.png"));
    rankBG->setScale(0.65);
    rankBG->setPos(275, 50);
    rankBG->setZValue(1);
    addItem(rankBG);
    rankBG->hide();
    rankon = false;

    /* init rank id */
    QFont rankFont("URW Chancery L", 25);
    rankFont.setBold(true);
    for(int i=0; i<10; i++)
    {
        rankID[i] = new QGraphicsSimpleTextItem(QString("%1.").arg(i+1));
        rankID[i]->setZValue(2);
        rankID[i]->setPos(310, 162+i*38.5);
        rankID[i]->setFont(rankFont);
        addItem(rankID[i]);
        rankID[i]->hide();
    }

    /* init rank name */
    for(int i=0; i<10; i++)
    {
        rankName[i] = new QGraphicsSimpleTextItem(QString("------"));
        rankName[i]->setZValue(2);
        rankName[i]->setPos(360, 162+i*38.5);
        rankName[i]->setFont(rankFont);
        addItem(rankName[i]);
        rankName[i]->hide();
    }

    /* init rank score */
    for(int i=0; i<10; i++)
    {
        rankScore[i] = new QGraphicsSimpleTextItem(QString("---"));
        rankScore[i]->setZValue(2);
        rankScore[i]->setPos(570, 162+i*38.5);
        rankScore[i]->setFont(rankFont);
        addItem(rankScore[i]);
        rankScore[i]->hide();
    }

    /* init bestScore */
    bestScore = new QGraphicsSimpleTextItem("Your Best: ");
    bestScore->setPos(305, 560);
    bestScore->setBrush(QBrush(QColor(Qt::white)));
    bestScore->setZValue(2);
    QFont bestScoreFont("URW Chancery L", 20);
    bestScoreFont.setItalic(true);
    bestScore->setFont(bestScoreFont);
    addItem(bestScore);
    bestScore->hide();

}

IndexScene::~IndexScene()
{
    delete indexBG;
    delete rankBG;
    delete startIcon;
    delete backIcon;
    delete rankIcon;
    delete startIconRect;
    delete rankIconRect;
    delete backIconRect;
    for(int i=0; i<10; i++)
    {
        delete rankName[i];
        delete rankScore[i];
        delete rankID[i];
    }

    delete bestScore;
}

void IndexScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(startIconRect->contains(event->scenePos().toPoint()) && !rankon)
    {
        if(!Mainwindow::soundMute)
          startIcon->playClickSound();
        emit pressStart();
    }
    else if(rankIconRect->contains(event->scenePos().toPoint()) && !rankon)
    {
        if(!Mainwindow::soundMute)
            rankIcon->playClickSound();
        showRank();
    }
    else if(backIconRect->contains(event->scenePos().toPoint()) && rankon)
    {
        if(!Mainwindow::soundMute)
            backIcon->playClickSound();
        hideRank();
    }
}

void IndexScene::resetIcon()    // recover unpressed state when scene change
{
    startIcon->setImage();
    backIcon->setImage();
}

void IndexScene::showRank() // start pos bug
{
    rankon = true;
    rankBG->show();
    backIcon->show();
    bestScore->show();
    for(int i=0; i<10; i++)
        rankName[i]->show(), rankID[i]->show(), rankScore[i]->show();
    updateRank();
    startIcon->setSoundFlag(false);
    rankIcon->setSoundFlag(false);
    backIcon->setSoundFlag(true);
}

void IndexScene::hideRank()
{
    rankon = false;
    rankBG->hide();
    backIcon->hide();
    bestScore->hide();
    resetIcon();
    for(int i=0; i<10; i++)
        rankName[i]->hide(), rankID[i]->hide(), rankScore[i]->hide();
    startIcon->setSoundFlag(true);
    rankIcon->setSoundFlag(true);
    backIcon->setSoundFlag(false);
}

void IndexScene::updateRank()
{
    QSqlQuery qry;
    QString name;
    /* set rank */
    if(qry.exec(QString("SELECT name, score FROM %1 ORDER BY score DESC").arg(tbName)))
    {
        int i=0;
        while(qry.next())
        {
//            qDebug() << qry.value(0).toString() << "\t" << qry.value(1).toInt();
            rankName[i]->setText(QString("%1").arg(qry.value(0).toString()));
            rankScore[i]->setText(QString("%1").arg(qry.value(1).toString()));
            if(i==9)
                break;
            i++;
        }
    }
    else
    {
        qDebug() << qry.lastError().text();
        return;
    }

    /* get userName */
    userName = dynamic_cast<Game*>(parent())->getuserName();

    /* set best score */
    if(qry.exec(QString("SELECT score FROM %1 WHERE name GLOB '%2' ORDER BY score DESC").arg(tbName, userName)))
        {
            qry.next();
            if(qry.value(0).toString().isEmpty())   // new player check
                bestScore->setText("Your Best:  0");
            else
                bestScore->setText(QString("Your Best:  %1").arg(qry.value(0).toString()));
    }
}

void IndexScene::polymorphismSetIconPixmap()
{
    Icon *icons[3];
    icons[0] = startIcon;
    icons[1] = rankIcon;
    icons[2] = backIcon;
    for(int i=0; i<3; i++)
        icons[i]->setImage();
}
