#include "IconRank.h"
#include "Mainwindow.h"

IconRank::IconRank(Icon::Type type, QGraphicsItem * parent)
    :Icon(type, parent)
{
    setAcceptHoverEvents(true);
    this->type = type;
}

IconRank::~IconRank()
{

}

void IconRank::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(inSoundFlag && !Mainwindow::soundMute)
        inSound->play();

    setPixmap(QPixmap(":/images/images/rank_icon_pressed.png"));
}

void IconRank::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setImage();
}

void IconRank::setImage()
{
    this->setPixmap(QPixmap(":/images/images/rank_icon.png"));
}

void IconRank::setType(Icon::Type type)
{
    this->type = type;
    setImage();
}
