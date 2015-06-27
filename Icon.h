#ifndef ICON_H
#define ICON_H
#include <QGraphicsPixmapItem>
#include <QSoundEffect>

class Icon : public QGraphicsPixmapItem
{
public:
    enum Type{START, RANK, BACK, AGAIN};
    Icon(Icon::Type type, QGraphicsItem * parent=0);
    ~Icon();
    virtual void setImage();
    Icon::Type getType();
    void playClickSound();
    void setSoundFlag(bool flag);
    virtual void setType(Icon::Type type) = 0;

protected:
    Type type;
    QSoundEffect *inSound;
    QSoundEffect *clickSound;
    bool inSoundFlag;
};

#endif // ICON_H
