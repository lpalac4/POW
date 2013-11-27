/** \file mapcreator.cpp
 *  @brief Definitions of MapCreator member functions.
 *  @author Joseph Katzin
 *  @author Andrew Ring
 */

#include <stdlib.h>
#include <QColor>
#include <unistd.h>
#include <QDebug>
#include <assert.h>

#include "mapcreator.h"
#include "cell.h"
#include "mapcreatortest.h"
#include "testingflags.h"
#include "nullplayer.h"
#include "utility.h"

using namespace std;

MapCreator::MapCreator(int numRows, int numCols, QObject *parent) : QObject(parent){
    this->numRows = numRows;
    this->numCols = numCols;
    //createBasicMap();
}

MapCreator::~MapCreator(){
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            delete(cellMap[row][col]);
        }
        delete(cellMap[row]);
    }
    delete(cellMap);
    delete(editor);
}

/**
 * @brief Populates cellMap with cells.
 */
void MapCreator::createBasicMap(void){
    int row;
    int col;
    editor = NULL;
    //allCells.clear();
    cellMap = new Cell** [numRows];
    for(row = 0; row < numRows; row++){
        cellMap[row] = new Cell* [numCols];
        for(col = 0; col < numCols; col++){
            Cell* hex = new Cell;
            hex->row = row;
            hex->col = col;
            //allCells.push_back(hex);
            cellMap[row][col] = hex;
        }
    }
}

/**
 * @brief Creates as many territories as specified in the constructor. Will create fewer
 * territories if it runs out of space on the map.
 */
void MapCreator::generate(void){
    vector<Cell*> frontier;
    Cell* hex = cellMap[rand() % numRows][rand() % numCols];
    hex->inFrontier = true;
    frontier.push_back(hex);
    unsigned int currentNumberOfTerritories = 0;

    while(currentNumberOfTerritories < MAX_TERRITORIES && !frontier.empty()){

        //As an alternative to the iteration through the frontier in newTerritory for removing
        //selected cells from the frontier, we could perform a check here with the inCountry boolean.
        if(newTerritory(frontier)){
            currentNumberOfTerritories++;
        }
        if(testingFlags::TEST_MAP_CREATOR) {
            mapGenTestSpace::printMap(create_FirstLevelRows());
        }
    }
}

/**
 * @brief A test to check which cells belong to which territories.
 * Colors the territories as though there were three players owning various territories. The colors allow
 * the territories to be shown at least somewhat distinctly.
 */
void MapCreator::randomlyColorTerritories()
{
    Player* p1 = new Player();
    Player* p2 = new Player();
    Player* p3 = new Player();

    p1->playerColorSet = new Color(qRgb(255, 0, 0), qRgb(150, 0, 0), qRgb(50, 0, 0));
    p2->playerColorSet = new Color(qRgb(0, 255, 0), qRgb(0, 150, 0), qRgb(0, 50, 0));
    p3->playerColorSet = new Color(qRgb(0, 0, 255), qRgb(0, 0, 150), qRgb(0, 0, 50));

    Player* nullP = NullPlayer::getNullPlayer();

    int count = 0;
    while(!nullP->territoryList->empty()) {
        Territory* p = nullP->territoryList->back();
        nullP->territoryList->pop_back();

        switch(count%3){
        case 0:
            p1->territoryList->push_back(p);
            p->ownedBy = p1;
            break;
        case 1:
            p2->territoryList->push_back(p);
            p->ownedBy = p2;
            break;
        case 2:
            p3->territoryList->push_back(p);
            p->ownedBy = p3;
            break;
        }
           count++;
    }
}

/**
 * @brief The high-level handle to create the map.
 */
void MapCreator::makeMeAMap(void){
    clock_t startTime;
    if(testingFlags::TEST_CALC_RUNTIME) {
        startTime = clock();
    }

    createBasicMap();
    generate();
    connectAllTerritories();
    if(testingFlags::TEST_COLOR_TERRITORIES_RANDOMLY) {
        randomlyColorTerritories();
    }

    Territory::calculateAllDistancesSimply();

    // We may want to move this...
    populateWithResourceNodes(4); // num 4 is temp!

    createMap();

    if(testingFlags::TEST_CALC_RUNTIME) {
        double elapsedTime = double(clock() - startTime)/CLOCKS_PER_SEC;
        qDebug() << "makeMeAMap() took " << elapsedTime << " seconds";
    }

}

/**
 * @brief Instantiates the MapEditor* editor field.
 */
void MapCreator::createMap(void){
    MapData* data = new MapData(create_FirstLevelRows(), numCols, numRows);
    editor = new MapEditor(data);
    editor->redraw();
    /*QObject::connect(this, SIGNAL(mapIsCreated(editor)), editor, SLOT(redraw()));
    emit mapIsCreated(editor);
    */

}

/**
 * @brief Creates a two-dimensional array of Territory*, with the first dimension
     * being row-addressible. This array represents the map, and which territory each cell belongs to.
     * This is the version that should be used.
     * @return A two-dimensional array of Territory*
     */
Territory*** MapCreator::create_FirstLevelRows(){
    Territory*** out;
    out = new Territory** [numRows];
    int row;
    for(row = 0; row < numRows; row++){
        out[row] = new Territory* [numCols];
        int col;
        for(col = 0; col < numCols; col++){
            out[row][col] = cellMap[row][col]->country;
        }
    }
    return out;
}

/**
 * @brief Creates a two-dimensional array of Territory*, with the first dimension
     * being column-addressible. This array represents the map, and which territory each cell belongs to.
     * This function should not be used.
     * @return A two-dimensional array of Territory*
     */
Territory*** MapCreator::create_FirstLevelCols(){
    Territory*** out;
    out = new Territory** [numCols];
    int col;
    for(col = 0; col < numCols; col++){
        out[col] = new Territory* [numRows];
        int row;
        for(row = 0; row < numRows; row++){
            out[col][row] = cellMap[row][col]->country;
        }
    }
    return out;
}

/**
 * @brief Cleanup operation to be called from newTerritory after
 * the latter has finished adding cells to the new territory. Ensures that no cell is in both the frontier
 * and the new territory.
 * Most likely, cells should already be removed from the frontier when added to a territory. This function
 * is defensive in nature.
 * @param inCountry The cells in the new territory
 * @param frontier The current frontier
 */
void MapCreator::removeCountryCellsFromFrontier(vector<Cell*> &inCountry, vector<Cell*> &frontier)
{
    vector<Cell*>::iterator inCountryIter;
    for(inCountryIter = inCountry.begin(); inCountryIter != inCountry.end(); ++inCountryIter) {
        Cell* currentCell = (*inCountryIter);

        unsigned int frontierIter;
        for(frontierIter = 0; frontierIter < frontier.size(); ++frontierIter) {
            while(frontierIter < frontier.size() && frontier.at(frontierIter) == currentCell) {
                frontier.erase(frontier.begin() + frontierIter);
            }
        }
    }
}

/**
 * @brief Cleanup operation to be called from newTerritory
 * after the latter has finished expanding the new territory. For each cell still in this list, resets the
 * isNeighbor property to false.
 * @param neighborList The list of neighbors from territory creation
 */
void MapCreator::clearNeighborSettingForUnpickedCells(vector<Cell*> &neighborList)
{
    unsigned int i;
    for(i = 0; i < neighborList.size(); i++){
        Cell* hex = neighborList.at(i);
        if(!hex->chosenForCountry){
            hex->isNeighbor = false;
        }
    }
}

/**
 * @brief Tries once to add a random cell from neighborList to the expanding
 * territory. Fails if that cell is already in a territory.
 * @param frontier The current frontier.
 * @param inCountry The cells already in country.
 * @param country The new territory.
 * @param neighborList The list of cells neighboring the new territory.
 */
void MapCreator::attemptToExpandCountry(vector<Cell*> &frontier, vector<Cell*> &inCountry,
                                        Territory* country, vector<Cell*> &neighborList)
{
    int index = rand() % neighborList.size();
    Cell* next = neighborList.at(index);
    neighborList.erase(neighborList.begin() + index);
    if(!next->chosenForCountry){
        next->country = country;
        next->isNeighbor = next->chosenForCountry = true;
        inCountry.push_back(next);
        neighbors(next, neighborList, frontier);
    }
}

/**
 * @brief Selects the seed for a new territory, currently from the frontier.
 * @note When we add islands and multiple continents, this will be the function to modify -- possibly the
 * only one to extensively change.
 * @param frontier The current frontier.
 * @return The selected seed.
 */
Cell* MapCreator::extractSeedFromList(vector<Cell*> &frontier)
{
    Cell* first;
    do {
        int index = rand() % frontier.size();
        first = frontier.at(index);
        frontier.erase(frontier.begin() + index);
    } while(first->chosenForCountry);

    return first;
}

/**
 * @brief Attempts to form a new territory using the group of cells passed as the
 * frontier.
 * @param frontier The current frontier of the map-building process overall. It will be modified over the
 * course of this functions running. After the function exits, the cells considered for the new
 * territory will no longer be in the frontier. Any neighbors of the new territory which are not already
 * in this or other territories will now be in the frontier.
 * @return true if a new territory was successfully produced. false if it
 * had to be made into water instead.
 */
bool MapCreator::newTerritory(vector<Cell*> &frontier){
    Cell* first = extractSeedFromList(frontier);

    Territory* country = new Territory;
    first->country = country;
    first->isNeighbor = first->chosenForCountry = true;
    vector<Cell*> inCountry;
    inCountry.push_back(first);

    vector<Cell*> neighborList;
    neighbors(first, neighborList, frontier);

    while(inCountry.size() < TERRITORY_SIZE && !neighborList.empty()){
        attemptToExpandCountry(frontier, inCountry, country, neighborList);
    }

    clearNeighborSettingForUnpickedCells(neighborList);

    removeCountryCellsFromFrontier(inCountry, frontier);

    if(inCountry.size() < TERRITORY_SIZE){
        delete(country);
        for(unsigned int i = 0; i < inCountry.size(); i++){
            inCountry.at(i)->country = NULL;
        }
        return false;
    }

    return true;
}

/**
 * @brief Adds the neighbors of tile to neighborList and frontier as needed
 * @param tile The cell to find neighbors of.
 * @param neighborList The neighbors of the expanding territory.
 * @param frontier The current frontier.
 */
void MapCreator::neighbors(Cell* tile, vector<Cell*> &neighborList, std::vector<Cell*> &frontier){
    vector<Utility::coordinatePair> listOfNewNeighbors =
            Utility::getNeighbors(tile->row, tile->col, numRows, numCols, FIRST_COL_UP);

    for(unsigned int iter = 0; iter < listOfNewNeighbors.size(); iter++){
        Utility::coordinatePair pair = listOfNewNeighbors.at(iter);
        Cell* hex = cellMap[pair.row][pair.col];
        if(!hex->isNeighbor && !hex->chosenForCountry){
            neighborList.push_back(hex);

            //if a regular shape is desired, this field will remain false. In that case,
            //a cell may be added multiple times to the neighborList. A cell is
            //thus more likely to be picked if it neighbors several previously picked
            //cells. This helps ensure regular shapes.
            if(!REGULAR_SHAPE){
                hex->isNeighbor = true;
            }

            //If we want to preferentially start new territories alongside old ones,
            //this allows a cell to be placed on the frontier multiple times.
            if(!hex->inFrontier || CLUMPED_TERRITORIES){
                frontier.push_back(hex);
                hex->inFrontier = true;
            }
        }
    }
}

/**
 * @brief Populates the adjacency list of each territory with its neighbors.
 */
void MapCreator::connectAllTerritories(){
    for(int row = 0; row < numRows; row++){
        for(int col = 0; col < numCols; col++){
            Cell* currentHex = cellMap[row][col];
            if(currentHex->country == NULL){
                continue;
            }
            vector<Utility::coordinatePair> listOfNewNeighbors =
                    Utility::getNeighbors(currentHex->row, currentHex->col,
                                          numRows, numCols, FIRST_COL_UP);
            for(unsigned int iter = 0; iter < listOfNewNeighbors.size(); iter++){
                Utility::coordinatePair pair = listOfNewNeighbors.at(iter);
                Cell* neighborHex = cellMap[pair.row][pair.col];
                if(neighborHex->country == NULL || neighborHex->country == currentHex->country){
                    continue;
                }
                if(!currentHex->country->isNeighbor(neighborHex->country)){
                    currentHex->country->adjacentTerritories->push_front(neighborHex->country);
                }
                if(!neighborHex->country->isNeighbor(currentHex->country)){
                    neighborHex->country->adjacentTerritories->push_front(currentHex->country);
                }
            }
        }
    }


}

/**
 * @brief Accessor function.
 * @return This object's MapEditor.
 */
MapEditor* MapCreator::getMapEditor(){
    return editor;
}

/**
 * @brief Populates the map with resource nodes.
 * @param numOfEach How many of each type of resource node to place.
 */
void MapCreator::populateWithResourceNodes(int numOfEach) {
    list<Territory*>* territoryList = NullPlayer::getNullPlayer()->territoryList;

    int listSize = territoryList->size();
    assert(listSize >= (numOfEach * (COUNT-1)));

    Territory* territories[listSize];

    list<Territory*>::iterator iter;
    int i = 0;
    for(iter = territoryList->begin();iter != territoryList->end(); ++iter, ++i) {
        territories[i] = (*iter);
    }

    for(int i = 0; i < numOfEach; i++) {
        // place one of each type of generator
        for(int genType = 1; genType < COUNT; genType++) {
            int choice = rand() % listSize;
            while(territories[choice]->resourceGen != NONE && territories[choice]->resourceGen != COUNT) {
                choice = rand() % listSize;
            }
            territories[choice]->resourceGen = (resourceGenerator)genType;
        }
    }

}
