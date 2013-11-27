#ifndef MAPCREATORTEST_H
#define MAPCREATORTEST_H

class Territory;

namespace mapGenTestSpace {
    const int NUM_ROWS = 10;
    const int NUM_COLS = 10;

    void printMap(Territory*** grid);
    void runBasicTest();
}

#endif // MAPCREATORTEST_H
