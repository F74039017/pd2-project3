#include "Icon.h"
#include <QDebug>

Icon::Icon(Icon::Type type, QGraphicsItem * parent)
    :QGraphicsPixmapItem(parent)
{
    this->type = type;

    /* enter icon sound */
    inSound = new QSoundEffect();
    inSound->setSource(QUrl("qrc:/sounds/sounds/icon_in.wav"));
    inSound->setVolume(0.8);
    inSoundFlag = false;

    /* click icon sound */
    clickSound = new QSoundEffect();
    clickSound->setSource(QUrl("qrc:/sounds/sounds/icon_click.wav"));
}

Icon::~Icon()
{
    delete inSound;
    delete clickSound;
}

void Icon::setImage()
{
    qDebug() << "call base";
}

Icon::Type Icon::getType()
{
    return type;
}

void Icon::playClickSound()
{
    clickSound->play();
}

void Icon::setSoundFlag(bool flag)
{
    this->inSoundFlag = flag;
}


