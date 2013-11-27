/** \file player.cpp
 *  @brief Definitions of Player class member functions.
 *  @author Andrew Ring
 *  @author Charles Myers
 */

#include "player.h"

std::list<Player*> Player::allPlayers;

/**
 * @brief Initializes name to empty string, and stash to an empty Resources.
 */
Player::Player(bool isAI)
{
    this->isAI = isAI;
    territoryList = new std::list<Territory*>;
    stash = new Resources();
    playerColorSet = 0;
    Player::allPlayers.push_back(this);
    std::ostringstream os;
    os << "Player " << Player::allPlayers.size();
    name = os.str();
}

Player::~Player() {
    delete(territoryList);
    Player::allPlayers.remove(this);
}

void Player::initializeAllForeignAffairs()
{
    std::list<Player*>::iterator outerIter;
    std::list<Player*>::iterator innerIter;

    for(outerIter = Player::allPlayers.begin(); outerIter != Player::allPlayers.end(); ++outerIter) {
        Player* playerBeingUpdated = (*outerIter);
        for(innerIter = Player::allPlayers.begin(); innerIter != Player::allPlayers.end(); ++innerIter) {
            Player* currentOtherPlayer = (*innerIter);
            if(playerBeingUpdated != currentOtherPlayer) {
                playerBeingUpdated->foreignAffairs[currentOtherPlayer] = NEUTRAL;
            }
        }
    }
}

/**
 * @brief Calculates the strength against the specified territory, excluding ally support and
 * reinforcements.
 * @param target The Territory strength should be calculated against.
 * @return The total strength against target, excluding ally support.
 */
int Player::getOffensiveStrength(Territory* target)
{
    int strength = 0;
    bool targetRammable = (target->cityPresent || target->wallPresent);

    std::list<Territory*>::iterator i;
    Territory* currentTerritory;
    for(i=target->adjacentTerritories->begin();i != target->adjacentTerritories->end(); ++i) {
        currentTerritory = (*i);
        if(currentTerritory->ownedBy == this) {
            strength += currentTerritory->getStrength();
            if(currentTerritory->ramPresent && targetRammable) {
                strength += strengths::ram;
            }
        }
    }

    return strength;
}


std::list<Territory*>* Player::collectResources()
{
    std::list<Territory*>* toUpdate = new std::list<Territory*>();

    std::list<Territory*>::iterator i;
    Territory* currentTerritory;
    for(i=territoryList->begin(); i != territoryList->end(); ++i) {
        currentTerritory = (*i);
        switch(currentTerritory->resourceGen) {
        case NONE:
            break;
        case HORSE:
        {
            Territory* location = currentTerritory->generateHorse();
            if(location) toUpdate->push_back(location);
        }
            break;
        case WOOD:
            stash->wood += currentTerritory->getNumResourcesGenerated();
            break;
        case IRON:
            stash->iron += currentTerritory->getNumResourcesGenerated();
            break;
        case STONE:
            stash->stone += currentTerritory->getNumResourcesGenerated();
            break;
        case GOLD:
            stash->gold += currentTerritory->getNumResourcesGenerated();
            break;
        case CROP:
            stash->crop += currentTerritory->getNumResourcesGenerated();
            break;
        case COUNT:
            // This should never happen!
            break;
        }
    }

    return toUpdate;
}

std::list<Territory*>* Player::allPlayersCollectResources()
{
    std::list<Territory*>* toUpdate;
    std::list<Territory*>* allToUpdate = new std::list<Territory*>();

    std::list<Player*>::iterator i;
    for(i = Player::allPlayers.begin(); i != Player::allPlayers.end(); ++i) {
        Player* currentPlayer = *i;
        toUpdate = currentPlayer->collectResources();
        while(toUpdate->size()) {
            allToUpdate->push_back(toUpdate->front());
            toUpdate->pop_front();
        }
        delete(toUpdate);
    }

    return allToUpdate;
}

unsigned int Player::numberOfCities()
{
    // This would be better if a count is maintained.  This needs accessors to achieve resonably.
    unsigned int count = 0;
    std::list<Territory*>::iterator itr;
    for(itr = territoryList->begin(); itr != territoryList->end(); ++itr) {
        Territory* currentTerritory = (*itr);
        if(currentTerritory->cityPresent)
            ++count;
    }
    return count;
}

/********** AI METHODS (Charles Myers) ***********/

Territory* Player::attackChoice(bool firstAttack){
    CombatComparator victoryCompare(this);
    std::priority_queue<Territory*, std::vector<Territory*>, CombatComparator>
            attackCandidates(victoryCompare);

    std::list<Territory*>::iterator territoryIterator;
    for(territoryIterator = Territory::allTerritoryList.begin();
        territoryIterator != Territory::allTerritoryList.end(); territoryIterator++){
        if((*territoryIterator)->ownedBy != this){
            attackCandidates.push(*territoryIterator);
        }
    }
    if(firstAttack){
        return attackCandidates.top();
    }else{
        if(attackCandidates.size() >= 2){
            attackCandidates.pop();
            return attackCandidates.top();
        }else{
            return NULL;
        }
    }
}

std::list<Territory*> Player::claimChoice(int numberToChoose){
    if(this->territoryList->size()){
        ClaimComparator selectCompare(this);
        std::priority_queue<Territory*, std::vector<Territory*>, ClaimComparator> claimCandidates(selectCompare);

        std::list<Territory*>::iterator territoryIterator;
        for(territoryIterator = Territory::allTerritoryList.begin();
            territoryIterator != Territory::allTerritoryList.end(); territoryIterator++){
            if((*territoryIterator)->ownedBy->name.compare("NullPlayer") == 0){
                claimCandidates.push(*territoryIterator);
            }
        }

        std::list<Territory*> claimChoices;
        for(int i=0; i<numberToChoose; i++){
            claimChoices.push_back(claimCandidates.top());
            claimCandidates.pop();
        }

        return claimChoices;
    }else{
        std::list<Territory*> firstChoice;
        std::list<Territory*>::iterator territoryIterator;
        for(territoryIterator = Territory::allTerritoryList.begin();
            territoryIterator != Territory::allTerritoryList.end(); territoryIterator++){

            if((*territoryIterator)->ownedBy->name.compare("NullPlayer") == 0 && (*territoryIterator)->resourceGen){
                firstChoice.push_back(*territoryIterator);
                break;
            }
        }
        if(territoryIterator == Territory::allTerritoryList.end()){
            for(territoryIterator = Territory::allTerritoryList.begin();
                territoryIterator != Territory::allTerritoryList.end(); territoryIterator++){

                if((*territoryIterator)->ownedBy->name.compare("NullPlayer") == 0){
                    firstChoice.push_back(*territoryIterator);
                    break;
                }
            }
        }
        return firstChoice;
    }
}

productionItemAndLocation Player::buildChoice(void){
    using namespace production;

    productionItemAndLocation whatIWant;
    whatIWant.item = (production::units) 0;
    whatIWant.location = NULL;

    Resources stashCopy(stash->stone, stash->gold, stash->iron, stash->crop, stash->wood);
    //Build as many cities as you can
    if(stashCopy.equalOrGreaterThan(&CITY_COST) || stashCopy.equalOrGreaterThan(&GOLD_CITY_COST)){
        if(stashCopy.equalOrGreaterThan(&CITY_COST)) whatIWant.item = CITY;
        else whatIWant.item = GOLD_CITY;
        Territory* citySpot = findCitySpot();
        if(citySpot){
            whatIWant.location = citySpot;
            return whatIWant;
        }
    }

    //If you'll be able to build a city next turn, set aside the resources you will need to do so.
    Resources whatIllGet;
    populateWhatIllGet(&whatIllGet);
    const Resources shortOfAGoldCity(0, GOLD_CITY_COST.gold - stashCopy.gold, 0, 0, 0);
    if(whatIllGet.equalOrGreaterThan(&shortOfAGoldCity)){
        stashCopy.gold = 0;
    }else{
        const Resources shortOfACity(stashCopy.stone < 1, stashCopy.gold < 1, stashCopy.iron < 1, stashCopy.crop < 1, stashCopy.wood < 1);
        if(whatIllGet.equalOrGreaterThan(&shortOfACity)){
            if(stashCopy.stone) stashCopy.stone--;
            if(stashCopy.gold) stashCopy.gold--;
            if(stashCopy.iron) stashCopy.iron--;
            if(stashCopy.crop) stashCopy.crop--;
            if(stashCopy.wood) stashCopy.wood--;
        }
    }

    //Consider building walls
    if(stashCopy.equalOrGreaterThan(&WALL_COST) || stashCopy.equalOrGreaterThan(&GOLD_WALL_COST)){
        Territory* needsDefence = lookForThreatenedCities();
        if(needsDefence){
            if(stashCopy.equalOrGreaterThan(&GOLD_WALL_COST)) whatIWant.item = GOLD_WALL;
            else whatIWant.item = WALL;
            whatIWant.location = needsDefence;
            return whatIWant;
        }
    }

    //Consider building weapons
    if(stashCopy.equalOrGreaterThan(&WEAPON_COST) || stashCopy.equalOrGreaterThan(&GOLD_WEAPON_COST)){
        if(stashCopy.equalOrGreaterThan(&GOLD_WEAPON_COST)) whatIWant.item = GOLD_WEAPON;
        else whatIWant.item = WEAPON;
        Territory* needsBeatdown = lookForAdjacentCities(WEAPON);
        if(needsBeatdown){
            whatIWant.location = needsBeatdown;
        }else{
            whatIWant.location = NULL;
            std::list<Territory*>::iterator myTerrIt;
            for(myTerrIt = territoryList->begin(); myTerrIt != territoryList->end(); myTerrIt++){
                if((*myTerrIt)->adjacentTerritories->size()){
                    whatIWant.location = (*myTerrIt);
                    break;
                }
            }
            if(!whatIWant.location){
                whatIWant.location = territoryList->front();
            }
        }
        return whatIWant;
    }

    //consider building rams
    if(stashCopy.equalOrGreaterThan(&RAM_COST) || stashCopy.equalOrGreaterThan(&GOLD_RAM_COST)){
        if(stashCopy.equalOrGreaterThan(&GOLD_RAM_COST)) whatIWant.item = GOLD_RAM;
        else whatIWant.item = RAM;
        Territory* needsBeatdown = lookForAdjacentCities(RAM);
        if(needsBeatdown){
            whatIWant.location = needsBeatdown;
            return whatIWant;
        }
    }

    //consider building silos
    if(stashCopy.equalOrGreaterThan(&SILO_COST) || stashCopy.equalOrGreaterThan(&GOLD_SILO_COST)){
        if(stashCopy.equalOrGreaterThan(&GOLD_SILO_COST)) whatIWant.item = GOLD_SILO;
        else whatIWant.item = SILO;

        Territory* siloLocationCandidate = findSiloLocation();
        if(siloLocationCandidate){
            whatIWant.location = siloLocationCandidate;
            return whatIWant;
        }
    }

    //there's nothing to build!
    productionItemAndLocation nullRequest;
    nullRequest.item = (production::units) 0;
    nullRequest.location = NULL;
    return nullRequest;
}

Territory* Player::findSiloLocation(void){
    std::list<Territory*>::iterator myTerrIt;
    for(myTerrIt = territoryList->begin(); myTerrIt != territoryList->end(); myTerrIt++){
        if((*myTerrIt)->resourceGen){
            return *myTerrIt;
        }
    }
    return NULL;
}

Territory* Player::lookForAdjacentCities(production::units weaponType){
    std::list<Territory*>::iterator myTerrIt;
    for(myTerrIt = territoryList->begin(); myTerrIt != territoryList->end(); myTerrIt++){
        if( ((!(*myTerrIt)->weaponPresent) && weaponType == production::WEAPON) || ((!(*myTerrIt)->ramPresent) && weaponType == production::RAM)){
            std::list<Territory*>::iterator adjListIt;
            for(adjListIt = (*myTerrIt)->adjacentTerritories->begin(); adjListIt != (*myTerrIt)->adjacentTerritories->end(); adjListIt++){
                if((*adjListIt)->ownedBy != this &&
                        ((*adjListIt)->cityPresent || ((*adjListIt)->wallPresent && weaponType == production::RAM))){
                    return *myTerrIt;
                }
            }
        }
    }
    return NULL;
}

Territory* Player::lookForThreatenedCities(void){
    std::list<Territory*>::iterator myTerrIt;
    for(myTerrIt = territoryList->begin(); myTerrIt != territoryList->end(); myTerrIt++){
        if((*myTerrIt)->cityPresent && !(*myTerrIt)->wallPresent){
            std::list<Territory*>::iterator adjListIt;
            for(adjListIt = (*myTerrIt)->adjacentTerritories->begin(); adjListIt != (*myTerrIt)->adjacentTerritories->end(); adjListIt++){
                if((*adjListIt)->ownedBy != this) return *myTerrIt;
            }
        }
    }
    return NULL;
}

Territory* Player::findCitySpot(void){
    int minOpposedAdjacent = -1;
    Territory* citySpot = NULL;

    std::list<Territory*>::iterator myTerrIt;
    for(myTerrIt = territoryList->begin(); myTerrIt != territoryList->end(); myTerrIt++){
        if(!(*myTerrIt)->cityPresent){
            int thisOpposedAdjacent = 0;
            std::list<Territory*>::iterator adjTerrIt;
            for(adjTerrIt = (*myTerrIt)->adjacentTerritories->begin(); adjTerrIt != (*myTerrIt)->adjacentTerritories->end(); adjTerrIt++){
                if((*adjTerrIt)->ownedBy != this) thisOpposedAdjacent++;
            }
            if(minOpposedAdjacent == -1 || minOpposedAdjacent > thisOpposedAdjacent){
                minOpposedAdjacent = thisOpposedAdjacent;
                citySpot = *myTerrIt;
            }
        }
    }
    return citySpot;
}

void Player::populateWhatIllGet(Resources* whatIllGet){
    using namespace production;
    whatIllGet->empty();

    std::list<Territory*>::iterator playerTerrIterator;
    for(playerTerrIterator = territoryList->begin(); playerTerrIterator != territoryList->end(); playerTerrIterator++){
        switch((*playerTerrIterator)->resourceGen){
        case WOOD:
            if((*playerTerrIterator)->cityPresent) whatIllGet->wood++;
            if((*playerTerrIterator)->siloPresent) whatIllGet->wood++;
            whatIllGet->wood++;
            break;
        case IRON:
            if((*playerTerrIterator)->cityPresent) whatIllGet->iron++;
            if((*playerTerrIterator)->siloPresent) whatIllGet->iron++;
            whatIllGet->iron++;
            break;
        case STONE:
            if((*playerTerrIterator)->cityPresent) whatIllGet->stone++;
            if((*playerTerrIterator)->siloPresent) whatIllGet->stone++;
            whatIllGet->stone++;
            break;
        case GOLD:
            if((*playerTerrIterator)->cityPresent) whatIllGet->gold++;
            if((*playerTerrIterator)->siloPresent) whatIllGet->gold++;
            whatIllGet->gold++;
            break;
        case CROP:
            if((*playerTerrIterator)->cityPresent) whatIllGet->crop++;
            if((*playerTerrIterator)->siloPresent) whatIllGet->crop++;
            whatIllGet->crop++;
            break;
        default:
            break;
        }
    }
}
