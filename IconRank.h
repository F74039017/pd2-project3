#ifndef ICONRANK_H
#define ICONRANK_H
#include "Icon.h"


class IconRank : public Icon
{
public:
    IconRank(Icon::Type type, QGraphicsItem * parent=0);
    ~IconRank();
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void setImage();
    virtual void setType(Icon::Type type);
};

#endif // ICONRANK_H
