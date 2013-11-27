/** \file edge.cpp
 *  @brief Definition of edge members.
 *  @author Charles Myers
 */

#include "edge.h"
/**
 * @brief Constructor for Edge.
 * @param xMin initial x coordinate
 * @param yMax maximum height of line
 * @param rise x component of slope
 * @param run y component of slope
 */
Edge::Edge(int xMin, int yMax, int rise, int run, QObject *parent) :
    QObject(parent)
{
    this->xMin = xMin;
    this->yMax = yMax;
    this->xFrac = 0;
    this->rise = rise;
    this->run = run;
    this->next = NULL;
}
/**
 * @brief getter for member xMin
 * @return this->xMin
 */
int Edge::getXMin(void){
    return xMin;
}
/**
 * @brief getter for member yMax
 * @return this->yMax
 */
int Edge::getYMax(void){
    return yMax;
}
/**
 * @brief getter for member xFrac
 * @return this->xFrac
 */
int Edge::getXFrac(void){
    return xFrac;
}
/**
 * @brief getter for member rise
 * @return this->rise
 */
int Edge::getRise(void){
    return rise;
}
/**
 * @brief getter for member run
 * @return this->run
 */
int Edge::getRun(void){
    return run;
}
/**
 * @brief getter for member next
 * @return this->next
 */
Edge* Edge::getNext(void){
    return next;
}

/**
 * @brief setter for member xMin
 * @param desired value for this->xMin
 */
void Edge::setXMin(int xMin){
    this->xMin = xMin;
}
/**
 * @brief setter for member yMax
 * @param desired value for this->yMax
 */
void Edge::setYMax(int yMax){
    this->yMax = yMax;
}
/**
 * @brief setter for member xFrac
 * @param desired value for this->xFrac
 */
void Edge::setXFrac(int xFrac){
    this->xFrac = xFrac;
}
/**
 * @brief setter for member rise
 * @param desired value for this->rise
 */
void Edge::setRise(int rise){
    this->rise = rise;
}
/**
 * @brief setter for member run
 * @param desired value for this->run
 */
void Edge::setRun(int run){
    this->run = run;
}
/**
 * @brief setter for member next
 * @param desired value for this->next
 */
void Edge::setNext(Edge* next){
    this->next = next;
}

