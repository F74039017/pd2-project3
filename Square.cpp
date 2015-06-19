#include "Square.h"
#include <QDebug>

Square::Square(QGraphicsItem * parent)
        :QGraphicsPixmapItem(parent)
{
    init();
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

//void Square::setExist(bool flag)
//{
//    exist = flag;
//    if(!flag)
//        hide();
//    else
//        show();
//}

//bool Square::isExist()
//{
//    return exist;
//}

//void Square::setcor(int x, int y)
//{
//    this->ix = x;
//    this->iy = y;
//}

//int Square::getX()
//{
//    return ix;
//}

//int Square::getY()
//{
//    return iy;
//}

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
    move->setDuration(250);  // 50 is moderate
    move->setStartValue(s);
    move->setEndValue(recoverPoint);
}

void Square::setDisapper()
{
    disappear = new QPropertyAnimation(this, "opacity", this);
    disappear->setDuration(250);
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

const Square &Square::operator=(const Square &x)    // only change type, need call updatePixmap();
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
                setPixmap(QPixmap(":/images/images/square2.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/square32.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/square64.png"));
            break;
        case WOOD:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/square4.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/square128.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/square256.png"));
            break;
        case THUNDER:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/square8.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/square512.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/square1024.png"));
            break;
        case WATER:
            if(effect == NO_EFFECT)
                setPixmap(QPixmap(":/images/images/square16.png"));
            else if(effect == VERTICAL)
                setPixmap(QPixmap(":/images/images/square2048.png"));
            else if(effect == HORIZON)
                setPixmap(QPixmap(":/images/images/squareX.png"));
            break;
        case BOMB:
            setPixmap(QPixmap(":/images/images/squareX.png"));
            break;
        case STAR:
            setPixmap(QPixmap(":/images/images/squareX.png"));
            break;
    }
}
