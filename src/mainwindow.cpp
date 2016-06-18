#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QDragMoveEvent>
#include "pig.h"
#include "pigcontactlistener.h"
#include "font.h"
#include <unistd.h>
#include <sstream>

using namespace std;

 //defining that 1 meter corresponds to x pixels
float  pfactor =   15.0f;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),score(0),birdSequence(2)
{
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene    

    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);

    // Create world
    //  create x direction / y direction acceleration
    world = new b2World(b2Vec2(0.0f, -9.8f));


    //create background pictures
    QGraphicsPixmapItem * map = new QGraphicsPixmapItem;
    map->setPixmap(QPixmap(":/bg.png").scaled(1250,600));
    map->setPos(0,0);
    scene->addItem(map);

     // create slinghot pictures
     QGraphicsPixmapItem * slingshot = new QGraphicsPixmapItem;
     slingshot ->setPixmap(QPixmap(":/slingshot.png").scaled(100,170));
     slingshot ->setPos(175,400);
     scene->addItem( slingshot );


    // create score text on the screen
    scoreText = new QGraphicsTextItem;
    scoreText->setPos(900,50);
    Font * font = new Font;  // install angry bird fonts
    scoreText->setFont(*font->fontType);
    scene->addItem(scoreText);


    // Setting Size of world & window and pass them to GameItem
    float w=size().width()/pfactor;
    float h = size().height()/pfactor;
    GameItem::setGlobalSize(QSizeF(w,h),size());



     //  /////////////////////////////////////////////////////////////Create ground & obstacles/////////////////////////////////////////////////////////////////////////////

    // land
    Land * land =  new Land(42,1.5,84,3,QPixmap(":/ground.png").scaled(84*pfactor,3*pfactor),world,scene);

    // stones
    Land * stone1 = new Land(60,4,40,2,QPixmap(":/stone.png").scaled(40*pfactor,2*pfactor ),world,scene);
    Stone * stone2 = new Stone(48.5,10,3,10,&timer,QPixmap(":/stone.png").scaled(3*pfactor,10*pfactor ),world,scene);
    Stone * stone3 = new Stone(71.5,10,3,10,&timer,QPixmap(":/stone.png").scaled(3*pfactor,10*pfactor ),world,scene);
    Stone * stone4 = new Stone(60,16,30,2,&timer,QPixmap(":/stone.png").scaled(30*pfactor,2*pfactor ),world,scene);

    // base
    base  *bb = new base(59.5,25,1,0.5,QPixmap(":/box2.png").scaled(1*pfactor,0.5*pfactor),world,scene);

    //anchorx,anchory represents the point of anchor
    anchorx =15;
    anchory = 12;

    //push items into lists
    itemList.push_back(land);
    itemList.push_back(bb);


    /* could be used to create a lots of piggys
   ostringstream stream;
    stream << "Mixed data, like this int: " << 137;
    string result = stream.str();
    cout << result << endl;
    */

    // //////////////////////////////////////////////////////////////create characters!!!!//////////////////////////////////////////////////////////////////

    Bird::setAnchorPosition(anchorx,anchory);

    Bird*birdie2 = new RedBird(anchorx,anchory,1.0f,&timer,QPixmap(":/redBird.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,land);
    Bird*birdie3 = new BlueBird(7.5,3,1.0f,&timer,QPixmap(":/blueBird.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,land,activeBirdList);
    Bird*birdie4 = new YellowBird(4.5,3,1.0f,&timer,QPixmap(":/yellowBird.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,land);
    Bird*birdie5 = new BigBird(1.5,3,2.0f,&timer,QPixmap(":/bigBird.png").scaled(4.0f*pfactor,4.0f*pfactor),world,scene,land);
    Pig*pig1 =new Pig(60,6,1.0f,&timer,QPixmap(":/pig.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,"PigA");
    Pig*pig2 =new Pig(55,6,1.0f,&timer,QPixmap(":/pig.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,"PigB");
    Pig*pig3 =new Pig(65,6,1.0f,&timer,QPixmap(":/pig.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,"PigC");
    Pig*pig4 =new Pig(60,27,1.0f,&timer,QPixmap(":/pig.png").scaled(2.0f*pfactor,2.0f*pfactor),world,scene,"PigD");
    //Pig*pig1 =new Pig(27,6,0.27f,12,&timer,QPixmap(":/pig.png").scaled(height()/9.0,height()/9.0),world,scene);


    // push birds / pigs into lists for use
    birdList.push_back(birdie2);
    birdList.push_back(birdie3);
    birdList.push_back(birdie4);
    birdList.push_back(birdie5);

    piglist.push_back(pig1);
    piglist.push_back(pig2);
    piglist.push_back(pig3);
    piglist.push_back(pig4);


   //initialize the activebirdlist & aim the activebird at right bird
   activeBirdList.push_back(birdList.takeFirst());
   activeBird = activeBirdList.at(0);

    // to trigger events
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));


    //set contactlistener
    PigContactListener *listenerInstance = new PigContactListener(piglist,activeBirdList,deleteList,score,world) ;
    world->SetContactListener(listenerInstance);

    //create functional button
    rbutton  = new RestartButton(5,35,5,5,QPixmap(":/restart.png").scaled(5*pfactor,5*pfactor ),world,scene) ;
    qbutton  = new QuitButton(12,35,5,5,QPixmap(":/close.png").scaled(5*pfactor,5*pfactor ),world,scene) ;

    timer.start(100/6);
}

// create and load birds ont the slingshot
void MainWindow::loadBird(){
    Bird*currBird;
    switch(birdSequence){
    case 2:
        currBird =   new BlueBird(anchorx,anchory,1.0f,&timer,QPixmap(":/blueBird.png").scaled(2*pfactor,2*pfactor),world,scene,itemList.first(),activeBirdList);
        break;

    case 3:
        currBird =   new YellowBird(anchorx,anchory,1.0f,&timer,QPixmap(":/yellowBird.png").scaled(2*pfactor,2*pfactor),world,scene,itemList.first());
        break;

    case 4:
        currBird =   new BigBird(anchorx,anchory,2.0f,&timer,QPixmap(":/bigBird.png").scaled(4*pfactor,4*pfactor),world,scene,itemList.first());

        break;
    }

    activeBirdList.push_back(currBird);
    birdSequence++;
}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{

    // Hint: Notice the Number of every event!
    if(event->type() == QEvent::MouseButtonPress)
    {
        // press the replay button
        if(   (  (cursorPos.x() > rbutton->getPosition1().x() && cursorPos.x() < rbutton->getPosition2().x()) && ( cursorPos.y() <rbutton->getPosition2().y() && cursorPos.y() > rbutton->getPosition1().y() ) ) ){
            qApp->exit(3);
        }
        // press the quitbutton
        else if( ((cursorPos.x() > qbutton->getPosition1().x() && cursorPos.x() <qbutton->getPosition2().x()) && ( cursorPos.y() <qbutton->getPosition2().y() && cursorPos.y() > qbutton->getPosition1().y() ) )){
          emit quitGame();
           qApp->exit(0);
        }

        // if not press on the functional button
        else{
            if(!activeBirdList.isEmpty()){     // if  there is still birds in the activeBirdlist , then the birds have two scenario , 1: wait to be shot  2. flying and able to attack
                if(!activeBird->getShooted()){      // 1.if the active bird  still not be shot
                    if(  (cursorPos.x() < activeBird->getPressPoint2().x() && cursorPos.x() > activeBird->getPressPoint1().x()) &&  // when we correctly clicked on the bird
                         ( cursorPos.y() <activeBird->getPressPoint2().y() && cursorPos.y() > activeBird->getPressPoint1().y()  )){
                        activeBird->setPressed(true);       // mousejoint we apply
                    }
                }
                else{                                                       //2. the bird is flying
                    if(!activeBird->getAttacked()){
                        activeBird->attack();
                    }
                 }
            }
        }
        return true;
    }


    if(event->type() == QEvent::MouseMove)
    {
    }

    if(event->type() == QEvent::MouseButtonRelease)
    {
        if(activeBird!=0){   //prevent from accessing dangling pointer
            if(!activeBird->getShooted()){   // when the bird hasn't been shot yet
               // if((cursorPos.x() < activeBird->getPressPoint2().x() && cursorPos.x() > activeBird->getPressPoint1().x()) && ( cursorPos.y() <activeBird->getPressPoint2().y() && cursorPos.y() > activeBird->getPressPoint1().y()  )){
                    if(activeBird->getPressed()){
                        activeBird->setPressed(false);
                        activeBird->fly();
                        activeBird->destroyJoint(world);  // we must destroythe joint to let the bird fly
                        activeBird->setShooted(true);   // be placed at the end could prevernt the bird from prematurely deleting

                    }
               // }
            }
        }
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

// the velocity to present the world
void MainWindow::tick()
{
    // we have to renew cursor point very often
    cursorPos.setX(QCursor::pos().x()-64);
    cursorPos.setY(QCursor::pos().y()-64);


    // set the score of games!!!
     scoreText->setPlainText(QString::number(score));

     // update the world
     world->Step(1.0/60,6,2);

     if(activeBird!=0){
        b2Vec2 activeBirdPos = activeBird->g_body->GetPosition();
        // important game process : deleting the bird!!
        if(  (activeBird->stop() && activeBird->getCollided())|| activeBirdPos.x>84 ||activeBirdPos.x<0 ){  // we have to check 1. wether the bird is stop moveing  2 . whether the bird has gone too far
            while(!activeBirdList.isEmpty()){
                deleteList.push_back(activeBirdList.takeFirst());   // if we meet the condition  above , we nedd to delte the current  bird
            }
            if(!birdList.isEmpty()){            // if there is still bird in the birdlist
                delete birdList.takeFirst();    // we have to delte the bird  in the birdlist first , then we use load() to reload a new bird on the slingshot . the bird in the birdlist is just for show up / lining up in the left corner
                loadBird();                             // load bird on the slingshot
                activeBird=activeBirdList.first();
            }
            else{
                activeBird =0;                      // for  safety , points the activBird to null
            }
        }
    }

     // constantly detecting whether something has to be deleted
    if(!deleteList.isEmpty()){
        GameItem* currItem;
        for(int i=0;i<deleteList.size();i++){
            currItem = deleteList.takeAt(i);
            delete currItem;
        }
    }
    scene->update();
}   

void MainWindow::QUITSLOT()
{
    // For debug//
    std::cout << "Quit Game Signal receive !" << std::endl ;
}
