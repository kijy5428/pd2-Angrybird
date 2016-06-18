#include "land.h"

Land::Land(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):StaticItem(x,  y,  w,  h,  pixmap, world, scene)
{



   // given the shape,density to define the body's fixture



       // /////////////////////////////////////////////////////////Create body of land /////////////////////////////////////////////////////////////////
    b2BodyDef bodyDef;
    Type* type  = new Type;
    type->category = -1;
    type->name ="land";
    bodyDef.userData = type;
    bodyDef.position.Set(x,y);
    g_body = world->CreateBody(&bodyDef);

    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);
    g_body->CreateFixture(&bodyBox,9.0f);

    paint();
}

