#ifndef UTILITY_H
#define UTILITY_H

#include <vector>

using namespace std;

namespace Utility
{
struct coordinatePair{
    int row;
    int col;
};
/**
 * @brief getNeighbors Calculates the neighbors to a given location.
 * @param row The row of the queried location.
 * @param col The column of the queried location.
 * @param numRows The total number of rows.
 * @param numCols The total number of columns.
 * @param firstColUp true if the even columns are higher than the odd ones.
 * @return The neighbors of the given location.
 */
vector<coordinatePair> getNeighbors(int row, int col, int numRows, int numCols, bool firstColUp);

}



#endif // UTILITY_H
