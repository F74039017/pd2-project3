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
    enum Type{FIRE, WOOD, THUNDER, WATER, STAR};
    enum Effect{VERTICAL, HORIZON, BOMB, NO_EFFECT};
    static const int typenum = 4;
    Square(QGraphicsItem * parent = 0);
    ~Square();
    void setType(Type value);
    Type getType();
    void setEffect(Effect value);
    Effect getEffect();
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
    virtual void updatePixmap();

private:
    Type type;
    Effect effect;
    QPointF recoverPoint;
    QPropertyAnimation *move;
    QPropertyAnimation *disappear;

    void init();
};

#endif // SQUARE

