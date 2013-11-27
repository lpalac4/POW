/** \file playerturnstatus.h
 *  @brief Declarations for the PlayerTurnStatus class.
 *  @author Andrew Ring
 */

#ifndef PLAYERTURNSTATUS_H
#define PLAYERTURNSTATUS_H

#include <map>
#include <assert.h>
#include "player.h"

using namespace std;

/**
 * @brief Used to update and maintain the current status of each Player during a turn.
 * @author Andrew Ring
 */
class PlayerTurnStatus
{
private:
    /**
     * @brief A map of the status of each Player.
     */
    map<Player*, bool> currentStatus;
public:
    /**
     * @brief Default constructor.
     */
    PlayerTurnStatus();

    /**
     * @brief Called to signal that the specified Player is done with their turn.
     * @param player The player which has completed their turn.
     */
    void setPlayerComplete(Player* player);

    /**
     * @brief Checks if the specified Player has signaled that they are finished with the current turn.
     * @param player The Player to check.
     * @return If the Player has completed their turn, True, else False.
     */
    bool getPlayerComplete(Player* player);

    /**
     * @brief Checks if all players have signalled completion of their turn.
     * @return If all players have completed their turn, True, else False.
     */
    bool allPlayersComplete();

    /**
     * @brief Resets the turn, such that all players have not completed it.
     */
    void clearTurn();
};

#endif // PLAYERTURNSTATUS_H
