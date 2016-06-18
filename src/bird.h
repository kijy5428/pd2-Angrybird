#ifndef BIRD_H
#define BIRD_H


#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>
#include <QPointF>
#include <gameitem.h>
#include "typeStruct.h"

#define BIRD_DENSITY 1.5f
#define BIRD_FRICTION 0.5f
#define BIRD_RESTITUTION 0.5f

class Bird : public GameItem
{
public:

    Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference);
    void setLinearVelocity(b2Vec2 velocity);
    void paint();
    void setPressed(bool flag);
    bool getPressed();
    void destroyJoint(b2World * world);
    void fly();
    QPointF getPressPoint1();
    QPointF getPressPoint2();
    bool stop();
    bool getShooted();
    void setShooted(bool flag);
    virtual void attack();
    bool getAttacked();
    static void setAnchorPosition(float x, float y);
    bool getCollided();
    void setCollided();

protected :
    bool pressed;
    b2MouseJoint * joint ;
    // (1) for calculating the velocity of birds  (2) for setting the initial position of birds
    static float anchorx,anchory;
    QPointF pressPoint1 ;
    QPointF pressPoint2 ;
    QPointF prevPoint;
    QPointF currPoint;
    bool shooted;
    bool collided;
    static int  birdPk;



protected:
    bool attacked;
    GameItem * reference;
    b2World *bworld;
    QGraphicsScene *bscene;
    QTimer * btimer;

};


class RedBird : public Bird
{
public:
    RedBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference);
    virtual void attack();
};

class BlueBird : public Bird
{
public:
    BlueBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference,QList<Bird*> &ActiveBirdList);
    virtual void attack();
    QList<Bird*> &activeBirdList;
   private :

};


class LittleBlueBird : public Bird
{
public:
    LittleBlueBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference);
  //  virtual void paint();
};


class YellowBird : public Bird
{
public:
    YellowBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference);
    virtual void attack();
};

class BigBird : public Bird
{
public:
    BigBird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,GameItem * reference);
    virtual void attack();
};

#endif // BIRD_H
