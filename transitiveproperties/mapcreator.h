/** \file mapcreator.h
 *  @brief Declarations for the MapCreator class as well as several constants.
 *  @author Joseph Katzin
 *  @author Andrew Ring
 */

#ifndef MAPCREATOR_H
#define MAPCREATOR_H

#include <QObject>
#include <vector>
#include <time.h>
#include "territory.h"
#include "mapdata.h"
#include "mapeditor.h"
#include "cell.h"

using namespace std;

//const bool FIRST_ROW_LEFT = true;
const bool FIRST_COL_UP = true;
const bool REGULAR_SHAPE = true;
const bool CLUMPED_TERRITORIES = true;
const unsigned int TERRITORY_SIZE = 9;
const unsigned int MAX_TERRITORIES = 40;

/**
 * @brief The MapCreator class controls the generation of the groups of hexes which are used as territories
 * @author Joseph "Yossi" Katzin
 * @author Andrew Ring (secondary)
 */
class MapCreator : public QObject
{
    Q_OBJECT

private:
    /**
     * @brief The number of rows of hexes
     */
    int numRows;
    /**
     * @brief The number of columns of hexes
     */
    int numCols;
    MapEditor *editor;
    //std::vector<cell*> allCells;
    /**
     * @brief This is treated as a two-dimensional array of Cell*. This data structure holds the internal
     * representation of the map.
     */
    Cell*** cellMap;
    /**
     * @brief Attempts to create a new territory using the frontier passed to it.
     * @param frontier The group of cells representing the frontier. Will be modified.
     * @return true if a territory was successfully formed. false upon failure
     */
    bool newTerritory(std::vector<Cell*> &frontier);
    /**
     * @brief Populates the cellMap with cells
     */
    void createBasicMap(void);
    /**
     * @brief A test.
     */
    void randomlyColorTerritories();
    /**
     * @brief A cleanup operation for territory creation
     * @param inCountry The cells chosen for the new territory
     * @param frontier The current frontier
     */
    void removeCountryCellsFromFrontier(std::vector<Cell *> &inCountry, std::vector<Cell*> &frontier);
    /**
     * @brief A cleanup operation for territory creation
     * @param neighborList The list of neighbors from territory creation
     */
    void clearNeighborSettingForUnpickedCells(std::vector<Cell *> &neighborList);
    /**
     * @brief Try once to add a cell to the expanding territory.
     * @param frontier The frontier
     * @param inCountry The cells already in the new territory
     * @param country The new territory
     * @param neighborList The cells bordering the new territory
     */
    void attemptToExpandCountry(std::vector<Cell*> &frontier, std::vector<Cell*> &inCountry, Territory* country, std::vector<Cell*> &neighborList);
    /**
     * @brief Selects a cell as the seed for a new territory.
     * @param frontier The current frontier.
     * @return A pointer to the selected cell.
     */
    Cell* extractSeedFromList(std::vector<Cell*> &frontier);
    /**
     * @brief Populates the adjacency list of each territory.
     */
    void connectAllTerritories(void);

public:

    MapCreator(int numRows, int numCols, QObject *parent = 0);
    ~MapCreator();
    /**
     * @brief Creates all territories.
     */
    void generate(void);
    /**
     * @brief Adds a cell's neighbors to the provided lists.
     * @param tile The cell to look for neighbors of.
     * @param neighborList The current list of neighbors to the expanding territory.
     * @param frontier The current frontier.
     */
    void neighbors(Cell* tile, std::vector<Cell*> &neighborList, std::vector<Cell*> &frontier);
    /**
     * @brief Instantiates the MapEditor* editor field.
     */
    void createMap(void);
    /**
     * @brief Creates a two-dimensional array of Territory*, with the first dimension
     * being row-addressible
     * @return A two-dimensional array of Territory*
     */
    Territory*** create_FirstLevelRows(void);
    /**
     * @brief Creates a two-dimensional array of Territory*, with the first dimension
     * being column-addressible
     * @return A two-dimensional array of Territory*
     */
    Territory*** create_FirstLevelCols(void);
    /**
     * @brief Accessor function
     * @return This object's MapEditor
     */
    MapEditor* getMapEditor();
    /**
     * @brief Populates the map with resource nodes.
     * @param numOfEach The number of each type of resource to place.
     */
    void populateWithResourceNodes(int numOfEach);

signals:
    /**
     * @brief Unused.
     * @param editor
     */
    void mapIsCreated(MapEditor *editor);

public slots:
    /**
     * @brief The high-level handle to create the map.
     */
    void makeMeAMap(void);
};

#endif // MAPCREATOR_H
