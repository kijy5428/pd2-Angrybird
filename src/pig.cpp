#include "pig.h"
#include<QTimer>
#include <QGraphicsScene>
#include <iostream>
using namespace std;

Pig::Pig(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,string pigName):GameItem(world)
{

    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(radius*2,radius*2);

    //  /////////////////////////////////////////////////////////////////Create Body!!!!!/////////////////////////////////////////////////////////////////////////
    //b2BodyDef gives the definition of body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = true;
    Type* type  = new Type;
    type->category= 100;
    type->name = pigName;
    bodydef.userData = type;
    // initial position
    bodydef.position.Set(x,y);


    // creating the body in the world using the bodydef (body definiton)
    // and pass to b2body pointer g_body
    g_body = world->CreateBody(&bodydef);

    // //////////////////////////////////set the properties of fixture////////////////////////////////////

    // set the shape of fixture as circle
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius;
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = 1;
    fixturedef.friction = 0.2;
    fixturedef.restitution = 0.3;

    g_body->SetAngularDamping(3);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);

    // Bound timer for painting objects
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    // add itself to the world
    scene->addItem(&g_pixmap);
}

