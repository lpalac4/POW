/** \file mapeditor.h
 *  @brief Declaration of the map-drawing class.
 *  @author Charles Myers
 */

#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QDebug>
#include <iostream>
#include <time.h>
#include <list>
#include <set>

#include "mapdata.h"
#include "graphicstool.h"
#include "territory.h"
#include "nullplayer.h"
#include "imagepathnames.h"
#include "testingflags.h"
/**
 * @brief The MapEditor class MapEditor takes the model of the map
 * from MapCreator and creates the QImage representing it. It can do
 * this either all at once (suitable for initial game set up) or one
 * territory at a time (suitable for highlighting selections).
 * @author Charles Myers
 */
class MapEditor : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief The QImage representing the map.
     */
    QImage* mapImage;
    /**
     * @brief The two dimensional array of Territory pointers,
     * which represents which hexes belong to which territories.
     */
    MapData* mapData;
    /**
     * @brief This is MapEditor's intance of the graphics tool,
     * which owns the scan-conversion method for polygons.
     */
    GraphicsTool* graphicsTool;
    /**
     * @brief There are a number of structures that need to be
     * built at set up and never again. This boolean is set
     * after they are constructed.
     */
    bool initialized;
    /**
     * @brief This enum is used to determine where borders are to
     * be drawn on hexes.
     */
    enum hexSide { TOP,
                   BOTTOM,
                   TOP_LEFT,
                   TOP_RIGHT,
                   BOTTOM_LEFT,
                   BOTTOM_RIGHT,
                   SIDE_COUNT };
    /**
     * @brief The number of pixels wide the image is.
     */
    int imageWidth;
    /**
     * @brief The number of pixels high the image is.
     */
    int imageHeight;
    /**
     * @brief The number of hexes wide the image is.
     */
    int hexesWide;
    /**
     * @brief The number of hexes high the image is.
     */
    int hexesHigh;
    /**
     * @brief The number of pixels high a hex is.
     */
    int hexPixelHeight;
    /**
     * @brief The number of pixels wide a hex is.
     */
    int hexPixelWidth;
    /**
     * @brief This is the increment along the x-axis
     * for each column of hexes.
     */
    int threeQuarterWidth;
    /**
     * @brief Each column is this much off along the
     * y-axis from its neighboring columns.
     */
    int halfHeight;
    /**
     * @brief The border variables are the dimensions
     * of the border trapezoids.
     */
    int borderWidth;
    int borderBase;
    int borderHypoteneuse;
    /**
     * @brief The length of the size of a square
     * inscribed within a hex.
     */
    int iconSquareSideLength;

    /**
     * @brief Called during the first execution of
     * redraw(). Initializes various parameters to be
     * used later.
     */
    void initializeMap(void);
    /**
     * @brief This method checks the hexes specified by
     * the coordinate parameters to see if they are owned
     * by the same player or not.
     * @param hexGrid The grid of territory pointers specifying
     * hex ownership.
     * @param x0 x coordinate of the first hex.
     * @param y0 y coordinate of the first hex.
     * @param x1 x coordinate of the second hex.
     * @param y1 y coordinate of the second hex.
     * @return True if the two hexes are not owned by the same player.
     */
    bool checkForOpposedTerritory(Territory*** hexGrid, int x0, int y0, int x1, int y1);

    /**
     * @brief This method determines the very first icon coordinates.
     * @param coordinates A result parameter to hold the x and y values.
     */
    void deriveIconCoordinates(int* coordinates);
    /**
     * @brief Finds the next unset pair of coordinates for this territory
     * and sets them.
     * @param coordinates Values to be set.
     * @param currentTerritory Territory containing the hex for this icon.
     */
    void setIconCoordinates(int* coordinates, Territory* currentTerritory);
    /**
     * @brief Checks the given territory's model for icons, and draws them
     * if present.
     * @param changedTerritory Pointer to given territory. If null, all
     * territories are checked for icons.
     */
    void drawIcons(Territory* changedTerritory);
    /**
     * @brief Low-level method that actually changes pixel values of the map.
     * @param imagePath Location of icon image.
     * @param x Pixel x coordinate.
     * @param y Pixel y coordinate.
     */
    void drawThisIconHere(const char* imagePath, int x, int y);
    /**
     * @brief Determines the vertices of the trapezoidal border on hexSide side.
     * @param hexVertices The vertices of the hex on which the border will be drawn.
     * @param side The side on which the border is to be drawn.
     * @param borderColor The color that the border should be.
     */
    void drawBorder(int hexVertices[6][2], hexSide side, QRgb borderColor);

public:
    /**
     * @brief Constructor! Yay!
     * @param mapData The model of hex ownership organized by territory.
     * @param parent Not important!
     */
    explicit MapEditor(MapData* mapData, QObject *parent = 0);

    /**
     * @brief An approximation of root three!!
     */
    static const double ROOT_THREE = 1.7320508;
    /**
     * @brief The configurable parameter on which hexPixelHeight is based.
     */
    static const int HEX_HEIGHT = 50;
    /**
     * @brief This constant determines the size of the borders as a ratio with
     * the hex height.
     */
    static const int HEXAGON_HEIGHT_TO_BORDER_HEIGHT_RATIO = 11;

    /**
     * @brief getter for the map image.
     * @return A pointer to the map image.
     */
    QImage* getMapImage(void);
    /**
     * @brief getter for the map model.
     * @return A pointer for the map model.
     */
    MapData* getMapData(void);

signals:
    /**
     * @brief Signal emitted when redraw() is done.
     * @param image Makes the map image available to the receiving slot.
     */
    void imageUpdated(QImage* image);
    void hexDrawn();
    
public slots:
    /**
     * @brief Main method of this class. If called without the territory
     * argument it will redraw all territories.
     */
    void redraw(void);
    void redraw(Territory* changedTerritory);
    void redraw(std::set<Territory*> changedTerritories);
};

#endif // MAPEDITOR_H
