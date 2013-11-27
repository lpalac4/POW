/** \file combatprocessor.h
 *  @brief Declaration of functions and constants in combat namespace.
 *  @author Andrew Ring
 */

#ifndef COMBATPROCESSOR_H
#define COMBATPROCESSOR_H

#include <stdlib.h>
#include <vector>
#include <QDebug>
#include <assert.h>
#include <vector>
#include <map>
#include <iostream>
#include <time.h>
#include "player.h"
#include "territory.h"
#include "testingflags.h"

/**
 * @brief A collection of tools for resolving combat between 2 or more players.
 * @author Andrew Ring
 */
namespace combat {

    /**
     * @brief A flag for enabling debug output.
     */
    const bool DEBUG_OUTPUT_ENABLED = true;

    /**
    * @brief The amount of random variance to be used in combat
    * resolution.  This will cause 0<=rand<COMBAT_RAND_VARIANCE to be added to each
    * players score.
    */
   const int COMBAT_RAND_VARIANCE = 2;

    /**
     * @brief Calculates the winner of a conflict.
     * @param contestingPlayers The players contesting the Territory.
     * @param contestedArea The Territory which is being attacked.
     * @return The Player who is victorious.
     */
    Player* resolve(std::vector<Player*>* contestingPlayers, Territory* contestedArea);
    /**
     * @brief A support function used by resolve() to calculate and add supporting strengths of
     * non-direct participants in a conflict.
     * @param contestingPlayers The players contesting the Territory.
     * @param contestedArea The Territory which is being attacked.
     * @param vals The map of players' strengths to be updated.
     */
    void addAllyStrengths(std::vector<Player*>* contestingPlayers, Territory* contestedArea,
                          std::map<Player*,int> &vals);
}

#endif // COMBATPROCESSOR_H
