#ifndef SQUARE
#define SQUARE

#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QPropertyAnimation>
#include <cstdlib>

class Square: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY (QPointF pos READ pos WRITE setPos)
    Q_PROPERTY (qreal opacity READ opacity WRITE setOpacity)

public:
    enum Type{FIRE, WOOD, THUNDER, WATER, BOMB, STAR};
    enum Effect{VERTICAL, HORIZON, NO_EFFECT};
    static const int typenum = 4;
    Square(QGraphicsItem * parent = 0);
    void setType(Type value);
    Type getType();
    void setEffect(Effect value);
    Effect getEffect();
//    void setExist(bool flag);
//    bool isExist();
//    void setcor(int x, int y);
//    int getX();
//    int getY();
    QPropertyAnimation *getMoveAnimation();
    QPropertyAnimation *getDisappearAnimation();
    void setMoveStart(QPointF s);
    void setDisapper();
    void recoverAnimation();
    void setRecoverPoint();
    bool operator==(Square const &x);
    bool operator!=(Square const &x);
    const Square &operator=(Square const &x);
    void randCreate();

private:
    Type type;
    Effect effect;
//    bool exist;
//    int ix;
//    int iy;
    QPointF recoverPoint;
    QPropertyAnimation *move;
    QPropertyAnimation *disappear;

    void init();
    virtual void updatePixmap();
};

#endif // SQUARE

