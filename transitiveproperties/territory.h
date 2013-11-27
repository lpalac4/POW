/** \file territory.h
 *  @brief Declaration of the Territory class, distanceAndTerritory struct and resourceGenerator enum.
 *  @author Andrew Ring
 */

#ifndef TERRITORY_H
#define TERRITORY_H

#include <list>
#include <vector>
#include <map>
#include <set>
#include <time.h>
#include <QDebug>

#include "water.h"
#include "player.h"
#include "testingflags.h"

class Water;
class Player;
//class NullPlayer;

/**
 * @brief The types of mutually exclusive resource generators which can exist on a territory.
 */
enum resourceGenerator { NONE,
                         WOOD,
                         IRON,
                         STONE, // Formerly Coal
                         GOLD,
                         CROP, // Formerly Wheat
                         HORSE,
                         COUNT };   // Cardinality of enum - Should always be last

struct distanceAndTerritory {
    Territory* territory;
    unsigned int distance;
};

/**
 * @brief A Territory and all of it's characteristics and behaviors.
 * @author Andrew Ring
 */
class Territory
{
private:
    /**
     * @brief Locate the nearest connected location for a horse.
     */
    Territory* findNearestWithNoHorse();
    static unsigned int** distanceArr;
    static bool distancesSet;
    static std::map<Territory*, int> territoryToDistanceIndex;
    static void setDistanceIndices(void);
    /**
     * @brief calculateDistances
     * @param newTerritory
     * @deprecated
     */
    static void calculateDistances(Territory* newTerritory);
    /**
     * @brief calculateDistances
     * @param newTerritory
     * @param frontier
     * @param visited
     * @param depthOfFrontier
     * @deprecated
     */
    static void calculateDistances(Territory* newTerritory, std::list<Territory*> &frontier, std::set<Territory*> &visited,
                                   int depthOfFrontier);
    static clock_t startTime;
public:
    /**
     * @brief Default constructor.
     */
    Territory();
    /**
     * @brief Destructor.
     */
    ~Territory();

    /**
     * @brief A self maintaining list of all the territories.
     */
    static std::list<Territory*> allTerritoryList;

    /**
     * @brief Removes all from the adjacency list, including removing self from neighbors adjacency lists.
     */
    void clearAdjacencies();

    /**
     * @brief Which, if any, resource is generated by this territory.
     */
    resourceGenerator resourceGen;

    // State Info
    /**
     * @brief Indicates if this territory is currently being by a city.
     */
    bool affectedByCity;
    /**
     * @brief The Player who controls this territory.
     */
    Player* ownedBy;
    /**
     * @brief If true, territory should be drawn highlighted, else draw normally.
     */
    bool selected;
    /**
     * @brief If the controlling Player has placed their stockpile on this territory.
     */
    bool stockpile;
    /**
     * @brief If there is a city here.
     */
    bool cityPresent;
    /**
     * @brief If there is a silo here.
     */
    bool siloPresent;
    /**
     * @brief If there is a weapon currently here.
     */
    bool weaponPresent;
    /**
     * @brief If there is a boat currently here.
     */
    bool boatPresent;
    /**
     * @brief If there is a horse currently here.
     */
    bool horsePresent;
    /**
     * @brief If there is a wall here.
     */
    bool wallPresent;
    /**
     * @brief If there is a ram currently here.
     */
    bool ramPresent;

    // Adjacency
    /**
     * @brief A list of territories which border this one.
     */
    std::list<Territory*>* adjacentTerritories;
    /**
     * @brief A list of Water areas which border this territory.
     */
    std::list<Water*>* adjacentWater;

    //Icon Location
    /**
     * @brief An array of pixel coordinates corresponding to the
     *        top left corner of where an icon should be located
     *        in a hex.
     */
    int iconLocalesOrganizedByTypeOfIcon[9][2];

    // Functions
    /**
     * @brief Rechecks the existence of a bordering city controlled by the owner of
     * this territory, and sets affectedByCity appropriately.
     */
    void cityCheck();
    /**
     * @brief Calculates the strength to defend this territory without ally support.
     * @return The defensive strength.
     */
    int getDefensiveStrength();
    /**
     * @brief Calculates the general individual strength of the territory.
     * @return The strength calculated.
     */
    int getStrength();
    /**
     * @brief Calculates the number of resources generated at this location per round.
     * @return The number of resources generated per round.
     */
    int getNumResourcesGenerated();
    /**
     * @brief This generates a horse unit on the nearest territory owned by the same player
     * if able.
     * @return The territory the horse was placed on.
     */
    Territory *generateHorse();

    /**
     * @brief Checks if the specified Territory is a neighbor of this one.
     * @param otherCountry The potential neighbor to be checked for.
     * @return True iff the specified Territory is a neighbor to this one, else False.
     */
    bool isNeighbor(Territory* otherCountry);

    /**
     * @brief Changes ownership of the territory.
     * @param newOwner The new owner to be set.
     */
    void changeOwnershipTo(Player* newOwner);

    /**
     * @brief Causes self and all adjacent territories to run cityCheck().
     */
    void radialCityCheck();

    /**
     * @brief <b>Deprecated.</b>  Use calculateDistancesSimply()
     * @deprecated Use calculateDistancesSimply()
     */
    static void calculateAllDistances(void);
    static int getDistance(Territory* a, Territory* b);
    static void calculateAllDistancesSimply();
    static void calculateDistancesSimply(Territory* from);
};

#endif // TERRITORY_H
