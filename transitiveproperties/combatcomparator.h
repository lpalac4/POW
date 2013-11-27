#ifndef COMBATCOMPARATOR_H
#define COMBATCOMPARATOR_H

#include "player.h"
#include "territory.h"

class Player;

class CombatComparator
{
public:
    CombatComparator(Player* attacker);
    bool operator() (Territory* &a, Territory* &b);
private:
    Player* attacker;
};

#endif // COMBATCOMPARATOR_H
