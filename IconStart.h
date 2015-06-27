#ifndef ICONSTART_H
#define ICONSTART_H
#include "Icon.h"


class IconStart : public Icon
{
public:
    IconStart(Icon::Type type, QGraphicsItem * parent=0);
    ~IconStart();
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void setImage();
    virtual void setType(Icon::Type type);
};

#endif // ICONSTART_H
