#ifndef ICONAGAIN_H
#define ICONAGAIN_H
#include "Icon.h"


class IconAgain : public Icon
{
public:
    IconAgain(Icon::Type type, QGraphicsItem * parent=0);
    ~IconAgain();
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void setImage();
    virtual void setType(Icon::Type type);
};

#endif // ICONAGAIN_H
