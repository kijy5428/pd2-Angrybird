#ifndef PIG_H
#define PIG_H
#include "gameitem.h"
#include <QGraphicsScene>
#include "typeStruct.h"

class Pig : public GameItem
{
public:
    Pig(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene,string pigName);

private :


};

#endif // PIG_H
