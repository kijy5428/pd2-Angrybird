#include "staticitem.h"


StaticItem::StaticItem(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
   g_pixmap.setPixmap(pixmap);
   g_size = QSize(w,h);
   scene->addItem(&g_pixmap);

}
