#include "dynamicitem.h"


DynamicItem::DynamicItem(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
   g_pixmap.setPixmap(pixmap);
   g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
   g_size = QSize(w,h);
   scene->addItem(&g_pixmap);
}
