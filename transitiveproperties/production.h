/** \file production.h
 *  @brief Constant Resources costs used in production, as well as a unit enum.  All in production namespace.
 *  @author Andrew Ring
 *  @author Charles Myers
 */

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "resources.h"

/**
 * @brief Unit type and cost values for use in production.
 * @author Andrew Ring
 */
namespace production {

/**
 * @brief All of the unit types.
 */
enum units {BOAT, CITY, RAM, SILO, WALL, WEAPON,
            GOLD_BOAT, GOLD_CITY, GOLD_RAM, GOLD_SILO, GOLD_WALL, GOLD_WEAPON,
            COUNT};

/**
 * @brief The cost to build a boat.
 */
const Resources BOAT_COST(0, 0, 0, 1, 3);
/**
 * @brief The cost to build a city.
 */
const Resources CITY_COST(1, 1, 1, 1, 1);
/**
 * @brief The cost to build a ram.
 */
const Resources RAM_COST(0, 0, 1, 0, 2);
/**
 * @brief The cost to build a silo.
 */
const Resources SILO_COST(1, 0, 0, 1, 0);
/**
 * @brief The cost to build a wall.
 */
const Resources WALL_COST(2, 0, 1, 0, 0);
/**
 * @brief The cost to build a weapon.
 */
const Resources WEAPON_COST(0, 0, 2, 0, 1);

const Resources GOLD_BOAT_COST(0, 4, 0, 0, 0);
const Resources GOLD_CITY_COST(0, 5, 0, 0, 0);
const Resources GOLD_RAM_COST(0, 3, 0, 0, 0);
const Resources GOLD_SILO_COST(0, 2, 0, 0, 0);
const Resources GOLD_WALL_COST(0, 3, 0, 0, 0);
const Resources GOLD_WEAPON_COST(0, 3, 0, 0, 0);

}

#endif // PRODUCTION_H
