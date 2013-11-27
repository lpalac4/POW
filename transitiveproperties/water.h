/** \file water.h
 *  @brief Declaration of Water class.
 *  @author Andrew Ring
 */

#ifndef WATER_H
#define WATER_H

#include <list>
//#include "territory.h"

class Territory;

/**
 * @brief The Water class
 * @author Andrew Ring
 */
class Water
{
public:
    /**
     * @brief Default constructor.
     */
    Water();

    /**
     * @brief A list of territories which share a border with this area of water.
     */
    std::list<Territory*>* adjacentTerritories;
};

#endif // WATER_H
