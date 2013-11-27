/** \file player.h
 *  @brief Declarations for the Player class, productionItemAndLocation struct and ForeignAffairsStanding enum.
 *  @author Andrew Ring
 *  @author Charles Myers
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include "color.h"
#include "resources.h"
#include "strengths.h"
#include "territory.h"
#include "production.h"
#include "combatcomparator.h"
#include "claimcomparator.h"

class Territory;

/**
 * @brief All possible (mutually exclusive) standings towards another player.
 */
enum ForeignAffairsStanding { ASSIST, DEFEND, NEUTRAL, OPPOSE };

/**
 * @brief A list of these structs will be used to communicate
 * the choices of an AI player during their production phase.
 */
struct productionItemAndLocation{
    production::units item;
    Territory* location;
};

/**
 * @brief The collection of characteristics and behaviors which define a Player.
 * @author Andrew Ring
 */
class Player
{

private:
    /**
     * @brief Traverses the list of territories owned by this player and counts
     * the number of resources he/she'll get next turn.
     * @param whatIllGet A container to hold the amount of stuff I'll get next turn.
     */
    void populateWhatIllGet(Resources* whatIllGet);

public:
    /**
     * @brief Default constructor.
     */
    Player(bool isAI = false);

    /**
     * @brief Destructor.
     */
    ~Player();

    /**
     * @brief A self maintaining list of all Players.
     */
    static std::list<Player*> allPlayers;

    /**
     * @brief Creates base (NEUTRAL) values for all foreign affairs between players.  Call only after all Players are created.
     */
    static void initializeAllForeignAffairs();

    /**
     * @brief The name of the player.
     */
    std::string name;

    /**
     * @brief The set of colors which will be displayed for the player.
     */
    Color* playerColorSet;

    /**
     * @brief This flag is set if this player is AI.
     */
    bool isAI;

    /**
     * @brief A collection of all of the resources currently available to the player.
     */
    Resources* stash;

    /**
     * @brief A list of territories which are controlled by the player.
     */
    std::list<Territory*>* territoryList;
    /**
     * @brief The standing of the player towards each other player.
     */
    std::map<Player*, ForeignAffairsStanding> foreignAffairs;

    /**
     * @brief The strength of the player against a specific Territory, with no
     * ally support or reinforces.
     * @param target The Territory strength should be considered against.
     * @return The strength against the specified Territory.
     */
    virtual int getOffensiveStrength(Territory* target);

    /**
     * @brief This causes the player to add to their stash a resources for each generator in a
     * Territory they control.
     * @return A list of all territories which need to be visually updated (due to placement of a horse, etc.)
     */
    virtual std::list<Territory*> *collectResources();

    /**
     * @brief Causes each Player to collect resources from any Resource generator.
     * @return A list of all territories which need to be visually updated (due to placement of a horse, etc.)
     */
    static std::list<Territory*> *allPlayersCollectResources();

    /**
     * @brief Counts the number of cities owned by this Player.
     * @return The number of cities owned by this Player.
     */
    unsigned int numberOfCities();

    /* AI methods */

    /**
     * @brief Call this method to receive attack choice from an AI player
     * @return Where this AI player would like to attack
     */
    Territory* attackChoice(bool firstAttack);
    /**
     * @brief Call this method to receive a list of territory choices from an AI player
     * @return A list of territories this AI player has chosen
     */
    std::list<Territory*> claimChoice(int numberToChoose);

    /**
     * @brief Call this method during an AI player's production phase to ask what they
     * would like to build.
     * @return A list of units to build and where to build them.
     */
    productionItemAndLocation buildChoice(void);

    /**
     * @brief The following methods find the optimal locations for where
     * to build the units CITY, WALL, RAM or WEAPON, SILO, respectively.
     * @return The location choice as a territory pointer.
     */
    Territory* findCitySpot(void);
    Territory* lookForThreatenedCities(void);
    Territory* lookForAdjacentCities(production::units weaponType);
    Territory* findSiloLocation(void);

    /* END AI methods */

};

#endif // PLAYER_H
