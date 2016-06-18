#ifndef PIGCONTACTLISTENER_H
#define PIGCONTACTLISTENER_H
#include <Box2D/Box2D.h>
#include <QList>
#include "pig.h"

class PigContactListener : public  b2ContactListener
{

public:
    //PigContactListener();
    PigContactListener(QList<Pig*> list,QList<GameItem*> &deleteList, int & score, b2World *World);
    QList<Pig*>piglist ;
    QList<GameItem*> &deleteList ;
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact) ;
    Pig* findPig(QList<Pig *> &list, string name1);
    int & score;

private:
        b2World * world ;

};

#endif // PIGCONTACTLISTENER_H
