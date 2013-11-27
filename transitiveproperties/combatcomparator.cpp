#include "combatcomparator.h"

CombatComparator::CombatComparator(Player* attacker){
    this->attacker = attacker;
}

bool CombatComparator::operator ()(Territory *&a, Territory *&b){
    double ratioA = attacker->getOffensiveStrength(a) / (double) a->getDefensiveStrength();
    double ratioB = attacker->getOffensiveStrength(b) / (double) b->getDefensiveStrength();
    if(ratioA < ratioB){
        return true;
    }
    return false;
}

