#include "IconBack.h"
#include "Mainwindow.h"

IconBack::IconBack(Icon::Type type, QGraphicsItem * parent)
    :Icon(type, parent)
{
    setAcceptHoverEvents(true);
    this->type = type;
}

IconBack::~IconBack()
{

}

void IconBack::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(inSoundFlag && !Mainwindow::soundMute)
        inSound->play();

    setPixmap(QPixmap(":/images/images/back_icon_pressed.png"));
}

void IconBack::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setImage();
}

void IconBack::setImage()
{
    this->setPixmap(QPixmap(":/images/images/back_icon.png"));
}

void IconBack::setType(Icon::Type type)
{
    this->type = type;
    setImage();
}
