#ifndef STATICITEM_H
#define STATICITEM_H
#include "gameitem.h"
#include <QGraphicsScene>

class StaticItem : public GameItem
{
public:
    StaticItem(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

#endif // STATICITEM_H
