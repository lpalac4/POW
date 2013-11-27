#include "cell.h"

Cell::Cell(){
    nullify();
}

/**
 * @brief Resets the cell to initial values.
 */
void Cell::nullify(){
    country = NULL;
    row = 0;
    col = 0;
    isNeighbor = false;
    chosenForCountry = false;
    inFrontier = false;
}
