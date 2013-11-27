/** \file maptest.h
 *  @brief Small testing class.
 *  @author Charles Myers
 */

#ifndef MAPTEST_H
#define MAPTEST_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include "mapdata.h"
#include "territory.h"
#include <iostream>

class MapTest : public QObject
{
    Q_OBJECT

private:
    QPixmap* pixmap;

public:
    explicit MapTest(QObject *parent = 0);
    void initializeMeSomeTerritories(MapData* &mapData);
    void setPixmap(QPixmap* pixmap);
signals:

public slots:
    void loadMap(QImage* mapImage);
};


#endif // MAPTEST_H
