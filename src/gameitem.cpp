#include "gameitem.h"
#include <QDebug>
#include <iostream>
using namespace std;

GameItem::GameItem(b2World *world):
    g_body(NULL),g_world(world)
{

}

GameItem::~GameItem()
{
    g_world->DestroyBody(g_body);
}

QSizeF GameItem::g_windowsize = QSizeF();
QSizeF GameItem::g_worldsize = QSizeF();

void GameItem::setGlobalSize(QSizeF worldsize, QSizeF windowsize)
{
    g_worldsize = worldsize;
    g_windowsize = windowsize;
}


void GameItem::paint()
{
    b2Vec2 pos = g_body->GetPosition();
    QPointF mappedPoint;

    // object's x,y coordinates in the world  transform to the representation in QT
    mappedPoint.setX(((pos.x-g_size.width()/2) * g_windowsize.width())/g_worldsize.width());
    mappedPoint.setY((1.0f - (pos.y+g_size.height()/2)/g_worldsize.height()) * g_windowsize.height());

    g_pixmap.setPos(mappedPoint);

    // set all properties to default values
    g_pixmap.resetTransform();

    // reset the rotation according to the body in the world
    g_pixmap.setRotation(-(g_body->GetAngle()*180/3.14159));
}


QSizeF GameItem::getSize(){

    return size;

}
