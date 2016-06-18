#ifndef DYNAMICITEM_H
#define DYNAMICITEM_H
#include "gameitem.h"
#include <QGraphicsScene>

class DynamicItem : public GameItem
{
public:
    DynamicItem(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

#endif // DYNAMICITEM_H
