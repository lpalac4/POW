#ifndef CELL_H
#define CELL_H

#include "territory.h"

class MapCreator;

/**
 * @brief The Cell class represents an individual position on the map.
 * @author Joseph "Yossi" Katzin
 */
class Cell
{
private:
    /**
     * @brief The territory this cell belongs to.
     */
    Territory* country;
    /**
     * @brief This cell's row on the map.
     */
    int row;
    /**
     * @brief This cell's column on the map.
     */
    int col;
    /**
     * @brief Whether this cell is currently a candidate for expanding a new territory.
     */
    bool isNeighbor;
    /**
     * @brief Whether this cell is already selected for a territory. (Also true if it is water).
     */
    bool chosenForCountry;
    /**
     * @brief Whether this cell is a potential seed for starting a new territory.
     */
    bool inFrontier;
public:
    Cell();
    /**
     * @brief Resets the cell to initial values.
     */
    void nullify(void);
    friend class MapCreator;
};

#endif // CELL_H
