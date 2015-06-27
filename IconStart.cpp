#include "IconStart.h"
#include "Mainwindow.h"

IconStart::IconStart(Icon::Type type, QGraphicsItem * parent)
    :Icon(type, parent)
{
    setAcceptHoverEvents(true);
    this->type = type;

    // no call setImage() for test polymorphism in indexSceneClass;
    //    setImage();
}

IconStart::~IconStart()
{

}

void IconStart::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(inSoundFlag && !Mainwindow::soundMute)
        inSound->play();

    setPixmap(QPixmap(":/images/images/start_icon_pressed.png"));
}

void IconStart::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    setImage();
}

void IconStart::setImage()
{
    this->setPixmap(QPixmap(":/images/images/start_icon.png"));
}

void IconStart::setType(Icon::Type type)
{
    this->type = type;
    setImage();
}
