#include <iostream>
#include <map>
#include "mapcreatortest.h"
#include "mapcreator.h"
#include "territory.h"

namespace mapGenTestSpace {
void printMap(Territory*** grid)
{
    std::map<Territory*, char> indices;
    char territoryID = 'A';

    for(int row = 0; row < NUM_ROWS; row++) {
        for(int j = 0; j < 2; j++){
            if(j == 1){
                std::cout << "   ";
            }
            for(int col = 0; col < NUM_COLS; col++) {
                Territory* territory = grid[row][col];
                if(indices.count(territory) == 0) {
                //    std::cout << "adding address " << territory
                //              << " as " << territoryID << std::endl;
                    indices[territory] = territoryID++;
                }
                if(col % 2 == j){
                    std::cout << indices.at(territory) << "     ";
                }
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
/*
    std::map<Territory*, char>::iterator i;
    for(i = indices.begin(); i != indices.end(); ++i) {
        Territory* ptr = (*i).first;
        char letter = (*i).second;

        std::cout << letter << " -> " << ptr << std::endl;
    }*/
}

void runBasicTest() {
        MapCreator* mc = new MapCreator(NUM_ROWS, NUM_COLS);
        mc->makeMeAMap();
        Territory*** grid = mc->create_FirstLevelRows();

        printMap(grid);

        return;
    }
}
