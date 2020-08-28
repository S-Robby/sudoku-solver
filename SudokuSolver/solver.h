#pragma once
#ifndef Solver
#include <iostream>

class Solver {
private:

    uint8_t num_tab[81];            // certain digits
    bool tab[81][9];                // possible digits

    void firstFillPossible();
    void elimRows();
    void elimCols();
    void elimSquares();
    void onlyOneInRow();
    void onlyOneInCol();
    void onlyOneInSquare();
    bool fillNumbers();
    bool isCorrect();
    bool checkChanged(bool previous[81][9]);
    void findLowestPossibleCell(uint8_t coords[2]);
    void copyTable(bool fromTable[81][9], bool toTable[81][9]);
    void copyTable(uint8_t fromTable[81], uint8_t toTable[81]);
    void setNumber(uint8_t coords, uint8_t number);
    void getSquare(uint8_t* square, uint8_t id);
public:

    bool start();
    bool solve();
    void printOutTable();
    void printOutPossibles();
    Solver();
    Solver(uint8_t* table);
};

#endif