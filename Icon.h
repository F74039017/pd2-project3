#ifndef ICON_H
#define ICON_H
#include <QGraphicsPixmapItem>
#include <QSoundEffect>

class Icon : public QGraphicsPixmapItem
{
public:
    enum Type{START, RANK, BACK, AGAIN, CONT};
    Icon(Icon::Type type, QGraphicsItem * parent=0);
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    void setImage(Icon::Type type);
    Icon::Type getType();
    void playClickSound();
    void setSoundFlag(bool flag);
    void setType(Icon::Type type);

private:
    Type type;
    QSoundEffect *inSound;
    QSoundEffect *clickSound;
    bool inSoundFlag;
};

#endif // ICON_H
