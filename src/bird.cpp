#include "bird.h"
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
#include <cmath>
#include <sstream>

using namespace std;

//one meter corresponds to 15 pixels
float  pfactor2 = 15.0f;
float Bird::anchorx = 0;
float Bird::anchory = 0;
int Bird::birdPk =0;


Bird::Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem* Reference):GameItem(world),pressed(false),shooted(false),
    attacked(false),reference(Reference),bworld(world),bscene(scene),btimer(timer),collided(0)
{

    // Set pixmap , given the image of the bird
    g_pixmap.setPixmap(pixmap);

    //set the origin point for transformation , don't know eactly what it does~!
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);

    // set the size of bird , as a square in the world
    g_size = QSize(radius*2,radius*2);

    size = QSizeF(g_pixmap.boundingRect().width(),g_pixmap.boundingRect().height());

    //for detecting whether the body is moving or not
    currPoint.setX(x);
    currPoint.setY(y);

    //  /////////////////////////////////////////////////////////////////Create Body!!!!!/////////////////////////////////////////////////////////////////////////


    // Bound timer for painting objects
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    // add itself to the world
    scene->addItem(&g_pixmap);
    //set up the definition for a xxx joint


}

// set the initial velocity
void Bird::setLinearVelocity(b2Vec2 velocity)
{
    g_body->SetLinearVelocity(velocity);
}

bool Bird::stop(){

    return (  !(currPoint.x()-prevPoint.x()) && !( currPoint.y()-prevPoint.y() ) &&  shooted );

}

bool Bird::getShooted(){
    return shooted;
}

void Bird::setShooted(bool flag){
     shooted = flag;

 }

void Bird::paint(){

    // override the base class pain()
    if(pressed){
        QPointF poss= QCursor::pos();
        float ax ,ay;

        //  constrict the range of the birds on the slingshot!!
        float curposx,curposy;
        curposx = poss.x()-64;
        curposy = poss.y()-64;

        if(curposx < 70 )
            curposx  =70;
        else if( curposx >400)
            curposx = 400;

        if(curposy <300)
            curposy = 300;
        else if(curposy > 500)
            curposy = 500;

        ax = (curposx) * g_worldsize.width()/g_windowsize.width();
        ay =( curposy)*g_worldsize.height()/g_windowsize.height();

        b2Vec2 *currentmousepos = new b2Vec2(ax,g_worldsize.height()-ay);
        joint->SetTarget(*currentmousepos);
    }


    // object's x,y coordinates in the world  transform to the representation in QT

    b2Vec2 pos = g_body->GetPosition();
    QPointF mappedPoint;

    mappedPoint.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width()   );
    mappedPoint.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height() );

    pressPoint1.setX(mappedPoint.x());
    pressPoint1.setY (mappedPoint.y());

    pressPoint2.setX(mappedPoint.x()+size.width());
    pressPoint2.setY(mappedPoint.y()+size.height());

    prevPoint.setX(currPoint.x());
    prevPoint.setY(currPoint.y());

    currPoint.setX(mappedPoint.x());
    currPoint.setY(mappedPoint.y());

    // paint the object on the scene
    g_pixmap.setPos(mappedPoint.x(),mappedPoint.y());

    // set all properties to default values
    g_pixmap.resetTransform();

    // reset the rotation according to the body in the world
    g_pixmap.setRotation(-(g_body->GetAngle()*180/3.14159));


}

void Bird::setPressed(bool flag){
    pressed = flag;

}

bool Bird:: getPressed(){
    return pressed;

}

void Bird::destroyJoint(b2World * world){

    if(joint != 0) {
        world->DestroyJoint(joint);
        joint =0;

     }

}

void Bird::fly(){

    b2Vec2 pos = g_body->GetPosition();
    float vFactorx,vFactory;
    vFactorx = 4;
    vFactory =1.2*vFactorx;
    b2Vec2 flyVelocity = b2Vec2((anchorx-pos.x)*vFactorx,(anchory-pos.y)*vFactory);
    setLinearVelocity(flyVelocity);


}

QPointF Bird::getPressPoint1(){

    return pressPoint1;

}

QPointF Bird::getPressPoint2(){

    return pressPoint2;

}

void Bird::setAnchorPosition(float x, float y){
    anchorx = x;
    anchory = y;

}

bool Bird::getCollided(){
    return collided;
}
void Bird::setCollided(){
    collided =1;
}


void Bird::attack(){}

bool Bird :: getAttacked(){
    return attacked;

}

RedBird ::RedBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference):Bird( x,  y, radius,timer, pixmap, world, scene, reference)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = 1;

    // setting the attribute of birds
    Type* type  = new Type;
    type->category = 1;
    ostringstream stream;
    stream << "B" << birdPk++;
    string result = stream.str();
    type->name =result;
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
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;


    b2MouseJointDef jointDef;
    jointDef.bodyA = reference->g_body;
    jointDef.bodyB =g_body;

    b2Vec2 pos = g_body->GetPosition();
    jointDef.target.Set(pos.x,pos.y);
    jointDef.collideConnected = true;
    jointDef.maxForce = 5000;
    joint = (b2MouseJoint*)world->CreateJoint( &jointDef );

    g_body->SetAngularDamping(3);
    g_body->SetLinearDamping(0.1);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);

}

void RedBird ::attack(){

    g_body->ApplyLinearImpulse(b2Vec2(0,-100),g_body->GetWorldCenter(),true);
    attacked = true;

}

BlueBird ::BlueBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference,QList<Bird*> &ActiveBirdList):Bird( x,  y, radius,timer, pixmap, world, scene, reference)
  ,activeBirdList(ActiveBirdList)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = 1;

    // setting the attribute of birds
    Type* type  = new Type;
    type->category = 1;
    ostringstream stream;
    stream << "B" << birdPk++;
    string result = stream.str();
    type->name =result;
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
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;


    b2MouseJointDef jointDef;
    jointDef.bodyA = reference->g_body;
    jointDef.bodyB =g_body;

    b2Vec2 pos = g_body->GetPosition();
    jointDef.target.Set(pos.x,pos.y);
    jointDef.collideConnected = true;
    jointDef.maxForce = 5000;
    joint = (b2MouseJoint*)world->CreateJoint( &jointDef );

    g_body->SetAngularDamping(3);
    g_body->SetLinearDamping(0.1);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);

}

void BlueBird ::attack(){

    b2Vec2 pos =    g_body->GetPosition();
    b2Vec2 velocity = g_body->GetLinearVelocity();

    Bird*birdy1 = new LittleBlueBird(pos.x+1,pos.y+1,1.0f,btimer,QPixmap(":/blueBird.png").scaled(2*pfactor2,2*pfactor2),bworld,bscene,reference);
    Bird*birdy2 = new LittleBlueBird(pos.x+1,pos.y-1,1.0f,btimer,QPixmap(":/blueBird.png").scaled(2*pfactor2,2*pfactor2),bworld,bscene,reference);

    birdy1->setLinearVelocity(b2Vec2(velocity.x+1,velocity.y+1));
    birdy2->setLinearVelocity(b2Vec2(velocity.x+1,velocity.y-1));
    activeBirdList.push_back(birdy1);
    activeBirdList.push_back(birdy2);
    attacked = true;
}


LittleBlueBird ::LittleBlueBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference):Bird( x,  y, radius,timer, pixmap, world, scene, reference)
{
    // Set pixmap , given the image of the bird


    //  /////////////////////////////////////////////////////////////////Create Body!!!!!/////////////////////////////////////////////////////////////////////////
    //b2BodyDef gives the definition of body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = 1;

    // setting the attribute of birds
    Type* type  = new Type;
    type->category = 1;
    type->name ="bird";
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
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;


    g_body->SetAngularDamping(3);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);

    // Bound timer for painting objects
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

}

YellowBird ::YellowBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference):Bird( x,  y, radius,timer, pixmap, world, scene, reference)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = 1;

    // setting the attribute of birds
    Type* type  = new Type;
    type->category = 1;
    ostringstream stream;
    stream << "B" << birdPk++;
    string result = stream.str();
    type->name =result;
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
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;


    b2MouseJointDef jointDef;
    jointDef.bodyA = reference->g_body;
    jointDef.bodyB =g_body;

    b2Vec2 pos = g_body->GetPosition();
    jointDef.target.Set(pos.x,pos.y);
    jointDef.collideConnected = true;
    jointDef.maxForce = 5000;
    joint = (b2MouseJoint*)world->CreateJoint( &jointDef );

    g_body->SetAngularDamping(3);
    g_body->SetLinearDamping(0.1);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);
}

void YellowBird ::attack(){

    if(!attacked){
        g_body->ApplyLinearImpulse(b2Vec2(300,0),g_body->GetWorldCenter(),true);
        attacked = true;
    }
}

BigBird ::BigBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference):Bird( x,  y, radius,timer, pixmap, world, scene, reference)
{
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;

    // whether it could shoot through others objects
    bodydef.bullet = 1;

    // setting the attribute of birds
    Type* type  = new Type;
    ostringstream stream;
    stream << "B" << birdPk++;
    string result = stream.str();
    type->name =result;
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
    fixturedef.density = 10;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = 0.0;


    b2MouseJointDef jointDef;
    jointDef.bodyA = reference->g_body;
    jointDef.bodyB =g_body;

    b2Vec2 pos = g_body->GetPosition();
    jointDef.target.Set(pos.x,pos.y);
    jointDef.collideConnected = true;
    jointDef.maxForce = 1000000;
    joint = (b2MouseJoint*)world->CreateJoint( &jointDef );

    g_body->SetAngularDamping(3);
    g_body->SetLinearDamping(0.0);

    // add the fixture with properties set before
    g_body->CreateFixture(&fixturedef);
}

void BigBird ::attack(){

}
