#ifndef INDEXSCENE_H
#define INDEXSCENE_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include "IconStart.h"
#include "IconRank.h"
#include "IconBack.h"
#include <QtSql>

class IndexScene: public QGraphicsScene
{
    Q_OBJECT

public:
    IndexScene(QObject *parent=0);
    ~IndexScene();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void resetIcon();

signals:
    void pressStart();

private:
    QGraphicsPixmapItem *indexBG;
    QGraphicsPixmapItem *rankBG;
    IconStart *startIcon;
    IconRank *rankIcon;
    IconBack *backIcon;
    QRect *startIconRect;
    QRect *rankIconRect;
    QRect *backIconRect;
    bool rankon;
    QGraphicsSimpleTextItem *rankName[10];
    QGraphicsSimpleTextItem *rankScore[10];
    QGraphicsSimpleTextItem *rankID[10];
    QGraphicsSimpleTextItem *bestScore;
    QString userName;

    void showRank();
    void hideRank();
    void updateRank();
    void polymorphismSetIconPixmap();
};

#endif // INDEXSCENE_H
