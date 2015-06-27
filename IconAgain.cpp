#include "IconAgain.h"
#include "Mainwindow.h"

IconAgain::IconAgain(Icon::Type type, QGraphicsItem * parent)
    :Icon(type, parent)
{
    setAcceptHoverEvents(true);
    this->type = type;
}

IconAgain::~IconAgain()
{

}

void IconAgain::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(inSoundFlag && !Mainwindow::soundMute)
        inSound->play();

    setPixmap(QPixmap(":/images/images/again_icon_pressed.png"));
}

void IconAgain::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setImage();
}

void IconAgain::setImage()
{
    this->setPixmap(QPixmap(":/images/images/again_icon.png"));
}

void IconAgain::setType(Icon::Type type)
{
    this->type = type;
    setImage();
}
