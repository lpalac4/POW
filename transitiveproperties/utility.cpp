#include "utility.h"

using namespace std;

namespace Utility{
    /**
     * @brief getNeighbors Calculates the coordinates of the neighbors of a given location on the map.
     * @param row The row of the given location.
     * @param col The column of the given location.
     * @param numRows The total number of rows in the map.
     * @param numCols The total number of columns in the map.
     * @param firstColUp true if the even columns are higher than the odd ones.
     * @return A vector of coordinate pairs for the neighbors of the queried location.
     */
    vector<coordinatePair> getNeighbors(int row, int col, int numRows, int numCols, bool firstColUp){
        vector<coordinatePair> output;// = new vector<coordinatePair>;
        coordinatePair pair;

        if(row > 0){
            pair.row = row - 1;
            pair.col = col;
            output.push_back(pair);
        }

        if(row < numRows - 1){
            pair.row = row + 1;
            pair.col = col;
            output.push_back(pair);
        }

        if(col > 0){
            pair.row = row;
            pair.col = col - 1;
            output.push_back(pair);
        }

        if(col < numCols - 1){
            pair.row = row;
            pair.col = col + 1;
            output.push_back(pair);
        }

        if(row > 0 && ((firstColUp && col % 2 == 0) || (!firstColUp && col % 2 == 1))){
            if(col > 0){
                pair.row = row - 1;
                pair.col = col - 1;
                output.push_back(pair);
            }
            if(col < numCols - 1){
                pair.row = row - 1;
                pair.col = col + 1;
                output.push_back(pair);
            }
        }

        if(row < numRows - 1 && ((firstColUp && col % 2 == 1) || (!firstColUp && col % 2 == 0))){
            if(col > 0){
                pair.row = row + 1;
                pair.col = col - 1;
                output.push_back(pair);
            }
            if(col < numCols - 1){
                pair.row = row + 1;
                pair.col = col + 1;
                output.push_back(pair);
            }
        }

        return output;
    }

}
