/** \file combatResultStruct.h
 *  @brief A data structure for holding the results of combat.
 *  @author Andrew Ring
 */

#ifndef COMBATRESULTSTRUCT_H
#define COMBATRESULTSTRUCT_H

//#include "territory.h"
//#include "player.h"

class Player;
class Territory;

/**
 * @brief Holds the results of combat.
 */
struct combatResultStruct{
    Territory* contestedArea;
    Player* defender;
    Player* winner;
};

#endif // COMBATRESULTSTRUCT_H
