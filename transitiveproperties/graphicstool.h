/** \file graphicstool.h
 *  @brief Declaration of tool used for scan-conversion of hexes.
 *  @author Charles Myers
 */

#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>
#include <QImage>
#include <QColor>
#include "edge.h"
#include "territory.h"
/**
 * @brief This class holds all the methods for drawing the hexagons
 *        and line borders of the territories.
 */
class GraphicsTool : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief The number of pixels wide that the image is.
     */
    int imageWidth;
    /**
     * @brief The number of pixels high that the image is.
     */
    int imageHeight;
    /**
     * @brief Pointer to the image this GraphicsTool draws.
     */
    QImage* image;

    /**
     * @brief Perform one iteration of bubble sort on the
     *        edge list to get rid of minimal unsortedness.
     * @param activeEdge Pointer to head of edge list.
     */
    void bubbleOnce(Edge* &activeEdge);
    /**
     * @brief Increment the edges' x coordinates according
     *        to their slope and current fractional component.
     * @param activeEdge Pointer to head of edge list.
     */
    void incrementEdges(Edge* &activeEdge);
    void mergeEdges(Edge* &activeEdge, Edge* tableEdge);
    void clearActiveEdges(Edge* &activeEdge, int y);

    //helper methods for drawLine
    void drawNegativeSteepLine(int x0, int y0, int y1, int dy, int dx, QRgb color);
    void drawNegativeLowLine(int x0, int y0, int x1, int dy, int dx, QRgb color);
    void drawPositiveSteepLine(int x0, int y0, int y1, int dy, int dx, QRgb color);
    void drawPositiveLowLine(int x0, int y0, int x1, int dy, int dx, QRgb color);
    void drawVerticalLine(int x0, int y0, int y1, QRgb color);
    void drawHorizontalLine(int x0, int y0, int x1, QRgb color);

public:
    explicit GraphicsTool(QImage* image, int imageWidth, int imageHeight, QObject *parent = 0);
    int getImageWidth(void);
    int getImageHeight(void);

signals:
    
public slots:
    void drawPolygon(int vertices[][2], int numberOfVertices, QRgb color, Territory* currentTerritory = NULL, Territory*** pixelTerritoryMapping = NULL);
    void drawLine(int x0, int y0, int x1, int y1, QRgb color);
    void clear(QRgb clearColor);

};

#endif // GRAPHICSTOOL_H
