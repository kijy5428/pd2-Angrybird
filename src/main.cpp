#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int result;
    do{

        MainWindow *w = new MainWindow;
        int x=1260;
        int y=600;
        w->setFixedSize(x,y);
        w->show();
       result = a.exec();
       delete w;
     }while(result);   // if result return 0 , then program restart / else exit


    return 0;
}






