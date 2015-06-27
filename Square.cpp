#include "Square.h"
#include <QDebug>
#define AnimationSpeed 250

Square::Square(QGraphicsItem * parent)
        :QGraphicsPixmapItem(parent)
{
    init();
}

Square::~Square()
{

}

void Square::setType(Type x)
{
    type = x;
    updatePixmap();
}

Square::Type Square::getType()
{
    return type;
}

void Square::setEffect(Square::Effect x)
{
    effect = x;
    updatePixmap();
}

Square::Effect Square::getEffect()
{
    return effect;
}

QPropertyAnimation *Square::getMoveAnimation()
{
    return move;
}

QPropertyAnimation *Square::getDisappearAnimation()
{
    return disappear;
}

void Square::setMoveStart(QPointF s)
{
    move = new QPropertyAnimation(this, "pos", this);
    move->setDuration(AnimationSpeed);
    move->setStartValue(s);
    move->setEndValue(recoverPoint);
}

void Square::setDisapper()
{
    disappear = new QPropertyAnimation(this, "opacity", this);
    disappear->setDuration(AnimationSpeed);
    disappear->setStartValue(1.0);
    disappear->setEndValue(0.0);
}

void Square::recoverAnimation()
{
    setPos(recoverPoint);
    setOpacity(1.0);
}

void Square::setRecoverPoint()
{
    recoverPoint = pos();
}

bool Square::operator==(const Square &x)
{
    if(this->type == x.type)
        return true;
    else
        return false;
}

bool Square::operator!=(const Square &x)
{
    return !(*this == x);
}

const Square &Square::operator=(const Square &x)
{
    this->type = x.type;
    this->effect = x.effect;
    updatePixmap();

    return *this;
}

void Square::randCreate()
{
    int ran = rand()%4;
    type = static_cast<Square::Type>(ran);
    effect = NO_EFFECT;
    updatePixmap();
}

void Square::init()
{
//    exist = false;
    type = FIRE;
    effect = NO_EFFECT;
}

void Square::updatePixmap()
{    
    switch(type)
    {
        case FIRE:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/FireSquare.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/FireSquareV.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/FireSquareH.png"));
            else if(effect == BOMB)
                setPixmap(QPixmap(":/images/images/FireSquareB.png"));
            break;
        case WOOD:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/LeafSquare.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/LeafSquareV.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/LeafSquareH.png"));
            else if(effect == BOMB)
                setPixmap(QPixmap(":/images/images/LeafSquareB.png"));
            break;
        case THUNDER:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/ThunderSquare.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/ThunderSquareV.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/ThunderSquareH.png"));
            else if(effect == BOMB)
                setPixmap(QPixmap(":/images/images/ThunderSquareB.png"));
            break;
        case WATER:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/WaterSquare.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/WaterSquareV.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/WaterSquareH.png"));
            else if(effect == BOMB)
                setPixmap(QPixmap(":/images/images/WaterSquareB.png"));
            break;
        case STAR:
            setPixmap(QPixmap(":/images/images/StarSquare.png"));
            break;
    }
}
