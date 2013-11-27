/** \file mapdata.h
 *  @brief Declaration of storage class for the map model.
 *  @author Charles Myers
 */

#ifndef MAPDATA_H
#define MAPDATA_H

#include <QObject>
#include "territory.h"

class MapData : public QObject
{
    Q_OBJECT
private:
    Territory*** hexGrid;
    Territory*** pixelTerritoryMapping;
    int hexesWide;
    int hexesHigh;

public:
    explicit MapData(Territory*** hexGrid, int width, int height, QObject *parent = 0);
    Territory* whatDidITouch(int y, int x);
    
    Territory*** getHexGrid(void);
    Territory*** getPixelTerritoryMapping(void);
    int getHexesWide(void);
    int getHexesHigh(void);

    void setPixelTerritoryMapping(Territory*** pixelTerritoryMapping);

signals:
    
public slots:
    
};

#endif // MAPDATA_H
