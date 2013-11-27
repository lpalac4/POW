/** \file playerturnstatus.cpp
 *  @brief Definitions of PlayerTurnStatus member functions.
 *  @author Andrew Ring
 */

#include "playerturnstatus.h"

using namespace std;

PlayerTurnStatus::PlayerTurnStatus()
{
    list<Player*>::iterator i;
    for(i = Player::allPlayers.begin(); i != Player::allPlayers.end(); ++i) {
        Player* newPlayer = (*i);
        if(newPlayer) {
            currentStatus[newPlayer] = false;
        }
    }
}

void PlayerTurnStatus::setPlayerComplete(Player* player)
{
    assert(player != NULL);
    currentStatus[player] = true;
}

bool PlayerTurnStatus::getPlayerComplete(Player* player)
{
    assert(player != NULL);
    return currentStatus[player];
}

bool PlayerTurnStatus::allPlayersComplete()
{
    map<Player*, bool>::iterator i;
    for(i = currentStatus.begin(); i != currentStatus.end(); ++i) {
        if(!i->second) {
            return false;
        }

    }
    return true;
}

void PlayerTurnStatus::clearTurn()
{
    map<Player*, bool>::iterator i;
    for(i = currentStatus.begin(); i != currentStatus.end(); ++i) {
        currentStatus[i->first] = false;
    }
}
