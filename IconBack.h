#ifndef ICONBACK_H
#define ICONBACK_H
#include "Icon.h"


class IconBack : public Icon
{
public:
    IconBack(Icon::Type type, QGraphicsItem * parent=0);
    ~IconBack();
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void setImage();
    virtual void setType(Icon::Type type);
};

#endif // ICONBACK_H
