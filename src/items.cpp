#include "items.h"
#include <QPointF>
#include <iostream>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <QDebug>
using namespace std;

bool base::pressed = false;



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

base::base(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):StaticItem(x,  y,  w,  h,  pixmap, world, scene)
{

    // /////////////////////////////////////////////////////////Create body of land /////////////////////////////////////////////////////////////////
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.bullet = true;
    Type* type  = new Type;
    type->category= -1;
    type->name ="base";
    bodyDef.userData = type;
    bodyDef.position.Set(x,y);
    g_body = world->CreateBody(&bodyDef);

    //  /////////////////////////set the fixture////////////////////////
    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyBox;
    fixturedef.density = 0.0f;

    // given the shape,density to define the body's fixture
    g_body->CreateFixture(&fixturedef);

    // Bound timer for painting objects

    paint();
}



void base::setPressed(bool flag){
    pressed = flag;
}


Stone::Stone(float x, float y, float w, float h, QTimer * timer,QPixmap pixmap, b2World *world, QGraphicsScene *scene):DynamicItem(x,  y,  w,  h,  pixmap, world, scene)
{

    // /////////////////////////////////////////////////////////Create body of land /////////////////////////////////////////////////////////////////
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    Type* type  = new Type;
    type->category = 0;
    type->name ="stone";
    bodyDef.userData = type;
    bodyDef.position.Set(x,y);
    g_body = world->CreateBody(&bodyDef);

    //  /////////////////////////set the fixture////////////////////////
    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyBox;
    fixturedef.friction = 5;
    fixturedef.restitution = 5;

    // given the shape,density to define the body's fixture
    g_body->CreateFixture(&bodyBox,1.5f);

    // Bound timer for painting objects
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    // add itself to the world

    paint();
}

Button::Button(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):StaticItem(x,  y,  w,  h,  pixmap, world, scene)
{

    // /////////////////////////////////////////////////////////Create body of land /////////////////////////////////////////////////////////////////
    b2BodyDef bodyDef;

    Type* type  = new Type;
    type->category = 0;
    type->name ="restartbutton";
    bodyDef.userData = type;
    bodyDef.position.Set(x,y);
    g_body = world->CreateBody(&bodyDef);

    //  /////////////////////////set the fixture////////////////////////
    b2PolygonShape bodyBox;
    bodyBox.SetAsBox(w/2,h/2);

    // given the shape,density to define the body's fixture
    g_body->CreateFixture(&bodyBox,9.0f);

    b2Vec2 pos = g_body->GetPosition();
    position1.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width());
    position1.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height());

    position2.setX(position1.x()+g_pixmap.boundingRect().width());
    position2.setY(position1.y()+g_pixmap.boundingRect().height());



    paint();
}


QPointF Button:: getPosition1(){

    return position1;
}

QPointF Button:: getPosition2(){

    return position2;
}


QuitButton::QuitButton(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):Button(x, y, w, h, pixmap, world,scene)
{

}




RestartButton::RestartButton(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene):Button(x, y, w, h, pixmap, world,scene)
{

}



