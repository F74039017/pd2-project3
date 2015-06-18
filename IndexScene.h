#ifndef INDEXSCENE_H
#define INDEXSCENE_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include "Icon.h"
#include <QtSql>

class IndexScene: public QGraphicsScene
{
    Q_OBJECT

public:
    IndexScene(QObject *parent=0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void resetIcon();

signals:
    void pressStart();

private:
    QGraphicsPixmapItem *indexBG;
    QGraphicsPixmapItem *rankBG;
    Icon *startIcon;
    Icon *rankIcon;
    Icon *backIcon;
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
};

#endif // INDEXSCENE_H
