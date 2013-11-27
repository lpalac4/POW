/** \file maptest.cpp
 *  @brief Definition of map test.
 *  @author Charles Myers
 */

#include "maptest.h"

using namespace std;


MapTest::MapTest(QObject *parent) :
    QObject(parent){
}

void MapTest::initializeMeSomeTerritories(MapData* &mapData){

    Territory* terr1 = new Territory;
    Territory* terr2 = new Territory;
    Territory* terr3 = new Territory;
    Territory* terr4 = new Territory;

    terr1->ownedBy = new Player;
    terr1->ownedBy->playerColorSet = new Color(qRgb(0,0,255), qRgb(200, 0, 0), qRgb(150, 0, 0));
    terr3->ownedBy = terr1->ownedBy;

    Territory*** hexGrid = new Territory** [20];
    for(int i=0; i<20; i++){
        hexGrid[i] = new Territory* [40];
        for(int k=0; k<40; k++){
            if(i<5) hexGrid[i][k] = terr1;
            else if(i<10) hexGrid[i][k] = terr2;
            else if(i<15) hexGrid[i][k] = terr3;
            else hexGrid[i][k] = terr4;
        }
    }

    mapData = new MapData(hexGrid, 40, 20);
}

void MapTest::setPixmap(QPixmap* pixmap){
    this->pixmap = pixmap;
}

void MapTest::loadMap(QImage* mapImage){
    this->pixmap->convertFromImage(*mapImage, 0);
}
