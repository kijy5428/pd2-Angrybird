#include "pigcontactlistener.h"
#include <iostream>
#include "typeStruct.h"
#include <sstream>

using namespace std;

//PigContactListener::PigContactListener():deleteList(QList<GameItem*>){}

PigContactListener::PigContactListener(QList<Pig*> list,QList<Bird*>& list2,QList<GameItem*>  &DeleteList,int &Score,b2World * World):piglist(list),birdlist(list2),deleteList(DeleteList),world(World),score(Score){}

void PigContactListener::BeginContact(b2Contact* contact){
    Type* type1,*type2;
    type1 =  (Type*)(contact->GetFixtureA()->GetBody()->GetUserData()) ;
    type2 =  (Type*)(contact->GetFixtureB()->GetBody()->GetUserData()) ;
    int category1,category2;
    category1 = type1->category;
    category2 = type2->category;

    string name1,name2;
    name1= type1->name;
    name2  = type2->name;

// pig contact detection
    if(category1==100 || category2 ==100){        // collision between pigs
        if(category1== 100){     // fixture A is pig
            if(category2 != 100){         // fixture B is not  pig
                if(category2!=-1){          // fixture B is not pig and also not land , then we will do the delete efforts
                    Pig* currPig  = findPig(piglist,name1);
                    deleteList.push_back(currPig);
                    score+=5000;
                    if(category2==1){
                     Bird * bird1 = findBird(birdlist,name2);
                     if(bird1!=0)
                         bird1->setCollided();
                    }
                }
            }
            else{              //fixture B is also pig
                Pig* currPig1,*currPig2 ;
                currPig1 = findPig(piglist,name1);
                currPig2 = findPig(piglist,name2);
                deleteList.push_back(currPig1);
                deleteList.push_back(currPig2);
                score+=10000;
            }
        }
       else{         //fixture A  is not pig / fixture B is pig
            if(category1!=-1){
                Pig* currPig = findPig(piglist,name2);
                deleteList.push_back(currPig);
                score+=5000;
                if(category1==1){
                    Bird * bird1 = findBird(birdlist,name1);
                    if(bird1!=0)
                        bird1->setCollided();
                }
            }
        }
    }


    // bird contact detection
    if(category1==1 || category2 ==1){
        if(category1==1){
            if(category2==1){
                Bird* bird1 = findBird(birdlist,name1);
                Bird* bird2 = findBird(birdlist,name2);
                cout << name1 << name2 << endl;
                if(bird1!=0)
                    bird1->setCollided();
                if(bird2!=0)
                    bird2->setCollided();
            }
            else{
                if(category2!=100){
                    Bird * bird1 = findBird(birdlist,name1);
                    if(bird1!=0)
                        bird1->setCollided();
                }
            }
        }
        else{
            if(category1!=100){
                Bird* bird1 = findBird(birdlist,name2);
                if(bird1!=0)
                    bird1->setCollided();
            }
        }
    }

}
void PigContactListener::EndContact(b2Contact* contact) {}

Pig* PigContactListener::findPig(QList<Pig*> &list,string currName){
     string targetName;
      Type* targetType;
    for(int i=0 ; i<list.size();i++){
        targetType= (Type*)list.at(i)->g_body->GetUserData() ;
        targetName = targetType ->name;
        if(currName.compare(targetName)==0){
            return list.takeAt(i);
        }
    }
}

Bird* PigContactListener::findBird(QList<Bird*> list,string currName){
     string targetName;
     Type* targetType;
    for(int i=0 ; i<list.size();i++){
        targetType= (Type*)list.at(i)->g_body->GetUserData() ;
        targetName = targetType ->name;
        if(currName.compare(targetName)==0){
            return list.at(i);
        }
    }
    return 0;
}

