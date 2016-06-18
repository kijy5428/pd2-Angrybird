#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>

#include <gameitem.h>
#include <items.h>
#include <bird.h>
#include <pig.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend  class PigContactListener;
    Q_OBJECT

public:
    QList<Bird*>activeBirdList;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *, QEvent *event);
    void closeEvent(QCloseEvent *);
    void loadBird();

signals:
    void quitGame();

private slots:
    void tick();
    void QUITSLOT();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QList<Bird *> birdList;
    QList<Pig*> piglist;
    QList<GameItem*> deleteList;
    Bird* activeBird;
    QTimer timer;
    QGraphicsTextItem * scoreText,*scoreText2 ;
    QPointF cursorPos;
    Button * qbutton,*rbutton;
    float cx,cy;
    float anchorx,anchory;
    int score;
    int birdSequence;   //represents the bird to create
};

#endif // MAINWINDOW_H
