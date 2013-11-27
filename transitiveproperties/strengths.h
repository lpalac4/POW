/** \file strengths.h
 *  @brief Constant strengths of various units.  Used in combat. Located in the strengths namespace.
 *  @author Andrew Ring
 */

#ifndef STRENGTHS_H
#define STRENGTHS_H

/**
 * @brief A collection of combat strengths of different units.
 * @author Andrew Ring
 */
namespace strengths {
    /**
     * @brief The strength added by a city.
     */
    const int city = 2;
    /**
     * @brief The strength added by a horse.
     */
    const int horse = 1;
    /**
     * @brief The strength added by a weapon.
     */
    const int weapon = 3;
    /**
     * @brief The strength added by a boat.
     */
    const int boat = 2;
    /**
     * @brief The strength added by a ram.
     */
    const int ram = 3;
    /**
     * @brief The strength added by a wall.
     */
    const int wall = 3;
}

#endif // STRENGTHS_H
