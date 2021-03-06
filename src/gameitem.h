#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <Box2D/Box2D.h>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTransform>
#include <QtMath>


    //what is qobject??
class GameItem : public QObject
{
    Q_OBJECT
public:

    b2Body *g_body;
    GameItem(b2World *world);
    ~GameItem();
    static void setGlobalSize(QSizeF worldsize, QSizeF windowsize);
    QSizeF getSize();

public slots:
    virtual void paint();
    // TODO virtual void collide();

protected:

    QGraphicsPixmapItem g_pixmap;
    QSizeF g_size;
    QSizeF size;
    b2World *g_world;
    static QSizeF g_worldsize, g_windowsize;

};

#endif // GAMEITEM_H
