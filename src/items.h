#ifndef ITEMS_H
#define ITEMS_H

#include <gameitem.h>
#include <QGraphicsScene>
#include "typeStruct.h"
#include "staticitem.h"
#include "dynamicitem.h"


class Land : public StaticItem
{
public:
    Land(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

class base : public StaticItem
{
public:
   base(float x, float y, float w, float h, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
   static void  setPressed(bool flag);

private :
    b2MouseJoint * joint ;
    bool flag ;
    static bool pressed ;
};


class Stone : public DynamicItem
{
public:
    Stone(float x, float y, float w, float h,QTimer * timer,QPixmap pixmap, b2World *world, QGraphicsScene *scene);
};

class Button : public StaticItem
{
public:
    Button(float x, float y, float w, float h,QPixmap pixmap, b2World *world, QGraphicsScene *scene);

    QPointF getPosition1();
    QPointF getPosition2();
    QPointF  position1;
    QPointF  position2;

};


class RestartButton : public Button
{
public:
     RestartButton(float x, float y, float w, float h,QPixmap pixmap, b2World *world, QGraphicsScene *scene);

};



class QuitButton : public Button
{
public:
     QuitButton(float x, float y, float w, float h,QPixmap pixmap, b2World *world, QGraphicsScene *scene);

};



#endif // ITEMS_H
