/** \file mapdata.cpp
 *  @brief Definition of mapdata storage class members.
 *  @author Charles Myers
 */

#include "mapdata.h"

MapData::MapData(Territory*** hexGrid, int width, int height, QObject *parent) :
    QObject(parent)
{
    this->hexGrid = hexGrid;
    this->hexesWide = width;
    this->hexesHigh = height;
}

Territory* MapData::whatDidITouch(int y, int x){
    return *(*(pixelTerritoryMapping + y) + x);
}

Territory*** MapData::getHexGrid(void){
    return hexGrid;
}

Territory*** MapData::getPixelTerritoryMapping(void){
    return pixelTerritoryMapping;
}

int MapData::getHexesWide(void){
    return hexesWide;
}

int MapData::getHexesHigh(void){
    return hexesHigh;
}

void MapData::setPixelTerritoryMapping(Territory ***pixelTerritoryMapping){
    this->pixelTerritoryMapping = pixelTerritoryMapping;
}
