/** \file edge.h
 *  @brief Declaration of utility class for scan conversion.
 *  @author Charles Myers
 */

#ifndef EDGE_H
#define EDGE_H

#include <QObject>
/**
 * @brief Used by the graphics tool to keep track of edges of polygons.
 */
class Edge : public QObject
{
    Q_OBJECT
//TODO: doxygen commenting
private:
    /**
     * @brief The current x coordinate of this edge.
     */
    int xMin;
    /**
     * @brief The scan line at which this edge is complete.
     */
    int yMax;
    /**
     * @brief The fractional part of the x coordinate.
     */
    int xFrac;
    /**
     * @brief The change in y for the corresponding change in x.
     */
    int rise;
    /**
     * @brief The change in x for the corresponding change in y.
     */
    int run;
    /**
     * @brief A pointer to the next edge in the edge table.
     */
    Edge* next;

public:
    /**
     * @brief Constructor for Edge.
     * @param xMin initial x coordinate
     * @param yMax maximum height of line
     * @param rise x component of slope
     * @param run y component of slope
     */
    explicit Edge(int xMin, int yMax, int rise, int run, QObject *parent = 0);
    /**
     * @brief getter for member xMin
     * @return this->xMin
     */
    int getXMin(void);
    /**
     * @brief getter for member yMax
     * @return this->yMax
     */
    int getYMax(void);
    /**
     * @brief getter for member xFrac
     * @return this->xFrac
     */
    int getXFrac(void);
    /**
     * @brief getter for member rise
     * @return this->rise
     */
    int getRise(void);
    /**
     * @brief getter for member run
     * @return this->run
     */
    int getRun(void);
    /**
     * @brief getter for member next
     * @return this->next
     */
    Edge* getNext(void);

    /**
     * @brief setter for member xMin
     * @param desired value for this->xMin
     */
    void setXMin(int xMin);
    /**
     * @brief setter for member yMax
     * @param desired value for this->yMax
     */
    void setYMax(int yMax);
    /**
     * @brief setter for member xFrac
     * @param desired value for this->xFrac
     */
    void setXFrac(int xFrac);
    /**
     * @brief setter for member rise
     * @param desired value for this->rise
     */
    void setRise(int rise);
    /**
     * @brief setter for member run
     * @param desired value for this->run
     */
    void setRun(int run);
    /**
     * @brief setter for member next
     * @param desired value for this->next
     */
    void setNext(Edge* next);

signals:
    
public slots:
    
};

#endif // EDGE_H
