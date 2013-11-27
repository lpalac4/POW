#include "territory.h"

namespace testDataGenerator {

    Territory* generateSimpleTerritorySet(unsigned int numAdjacent) {
        Territory* coreTerritory = new Territory();
        coreTerritory->ownedBy = new Player();

        unsigned int i;
        Territory* temp;
        for(i = 0; i < numAdjacent; i++){
            temp = new Territory;
            temp->ownedBy = new Player();
            temp->adjacentTerritories->push_back(temp);
            coreTerritory->adjacentTerritories->push_back(temp);
        }

        return coreTerritory;
    }
}
