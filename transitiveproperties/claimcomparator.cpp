/** \file claimcomparator.cpp
 *  @brief Definition of comparison method used for territory selection.
 *  @author Charles Myers
 */

#include "claimcomparator.h"

ClaimComparator::ClaimComparator(Player* chooser)
{
    this->chooser = chooser;
}

bool ClaimComparator::operator ()(Territory *a, Territory *b){

    if(chooser->territoryList->size() == 0){
        if(a->resourceGen != NONE) return false;
        return true;
    }

    double averageDistanceA = 0;
    double averageDistanceB = 0;

    std::list<Territory*>::iterator chooserTerritory;
    for(chooserTerritory = chooser->territoryList->begin();
        chooserTerritory != chooser->territoryList->end();chooserTerritory++){

        averageDistanceA += Territory::getDistance(a, *chooserTerritory);
        averageDistanceB += Territory::getDistance(b, *chooserTerritory);
    }

    averageDistanceA /= chooser->territoryList->size();
    averageDistanceB /= chooser->territoryList->size();

    double aHasResource = 0;
    double bHasResource = 0;

    if(a->resourceGen) aHasResource = RESOURCE_WEIGHT;
    if(b->resourceGen) bHasResource = RESOURCE_WEIGHT;

    if((averageDistanceA - aHasResource) > (averageDistanceB - bHasResource)){
        return true;
    }
    return false;
}
