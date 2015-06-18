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

void Square::setExist(bool flag)
{
    exist = flag;
    if(!flag)
        hide();
    else
        show();
}

bool Square::isExist()
{
    return exist;
}

void Square::setcor(int x, int y)
{
    this->ix = x;
    this->iy = y;
}

int Square::getX()
{
    return ix;
}

int Square::getY()
{
    return iy;
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

const Square &Square::operator=(const Square &x)    // only change type, need call updatePixmap();
{
    this->type = x.type;
    updatePixmap();
}

void Square::init()
{
    exist = false;
    type = NON;
}

void Square::updatePixmap() //++
{
    switch(type)
    {
        case FIRE:
            setPixmap(QPixmap(":/images/images/square2.png"));
            break;
        case WOOD:
            setPixmap(QPixmap(":/images/images/square4.png"));
            break;
        case THUNDER:
            setPixmap(QPixmap(":/images/images/square8.png"));
            break;
        case WATER:
            setPixmap(QPixmap(":/images/images/square16.png"));
            break;
        case NON:
            setPixmap(QPixmap(":/images/images/squareX.png"));
    }
}
