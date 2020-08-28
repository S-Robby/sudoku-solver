#include "solver.h"

void Solver::firstFillPossible() {
    /*
    This function fills all the empty cells, assuming that
    every digit is possible to be an answer
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = y * 9 + x;

            if (num_tab[cell] == 0) {
                for (uint8_t i = 0; i < 9; i++) {
                    tab[cell][i] = 1;
                }
            }
        }
    }
}

void Solver::elimRows() {
    /*
    This function finds the certain digits and removes them
    as possible from all cells within the row
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = y * 9 + x;
            uint8_t number = num_tab[cell];

            if (number != 0) {                      //is there a certain number?
                for (uint8_t i = 0; i < 9; i++) {
                    tab[y * 9 + i][number - 1] = 0; //remove found number as possible from the row
                }
                tab[cell][number - 1] = 1;          //restore it as a possibility for that cell
            }
        }
    }
    fillNumbers();
}

void Solver::elimCols() {
    /*
    This function finds the certain digits and removes them
    as possible from all cells within the column
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = y * 9 + x;
            uint8_t number = num_tab[cell];

            if (number != 0) {
                for (uint8_t i = 0; i < 9; i++) {
                    tab[i * 9 + x][number - 1] = 0;
                }
                tab[cell][number - 1] = 1;
            }
        }
    }
    fillNumbers();
}

void Solver::elimSquares() {
    /*
    This function finds the certain digits and removes them
    as possible from all cells within the corresponding square
    */
    uint8_t square[9];

    for (uint8_t y = 0; y < 9; y++) {
        getSquare(square, y);

        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = square[x];
            uint8_t number = num_tab[cell];

            if (number != 0) {
                for (uint8_t i = 0; i < 9; i++) {
                    tab[square[i]][number - 1] = 0;
                }
                tab[cell][number - 1] = 1;
            }
        }
    }

    fillNumbers();
}

void Solver::onlyOneInRow() {
    /*
    If a number is considered an option only in one cell from the row
    then it is forced into that slot
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t i = 0; i < 9; i++) {
            uint8_t possible[2] = { 0, 0 };

            for (uint8_t x = 0; x < 9; x++) {
                uint8_t cell = y * 9 + x;
                if (tab[cell][i] == 1) {
                    possible[0]++;
                    possible[1] = x;
                }
            }
            if (possible[0] == 1) {
                uint8_t cell = y * 9 + possible[1];
                for (uint8_t a = 0; a < 9; a++) {
                    tab[cell][a] = 0;
                }
                tab[cell][i] = 1;
            }
        }
    }
    fillNumbers();
}

void Solver::onlyOneInCol() {
    /*
    If a number is considered an option only in one cell from the column
    then it is forced into that slot
    */
    for (uint8_t x = 0; x < 9; x++) {
        for (uint8_t i = 0; i < 9; i++) {
            uint8_t possible[2] = { 0, 0 };

            for (uint8_t y = 0; y < 9; y++) {
                uint8_t cell = y * 9 + x;
                if (tab[cell][i] == 1) {
                    possible[0]++;
                    possible[1] = y;
                }
            }
            if (possible[0] == 1) {
                uint8_t cell = possible[1] * 9 + x;
                for (uint8_t a = 0; a < 9; a++) {
                    tab[cell][a] = 0;
                }
                tab[cell][i] = 1;
            }
        }
    }
    fillNumbers();
}

void Solver::onlyOneInSquare() {
    /*
    If a number is considered an option only in one cell from the sqaure
    then it is forced into that slot
    */

    uint8_t square[9];

    for (uint8_t i = 0; i < 9; i++) {
        getSquare(square, i);
        for (uint8_t number = 0; number < 9; number++) {
            uint8_t possible[2] = { 0, 0 };

            for (uint8_t x = 0; x < 9; x++) {
                if (tab[square[x]][number] == 1) {
                    possible[0]++;
                    possible[1] = square[x];
                }
            }

            if (possible[0] == 1) {
                for (uint8_t x = 0; x < 9; x++) {
                    tab[possible[1]][x] = 0;
                }
                tab[possible[1]][number] = 1;
            }
        }
    }

    fillNumbers();
}

bool Solver::fillNumbers() {
    /*
    When only one number is possible in a slot, fill that number
    to the num_tab
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = y * 9 + x;
            uint8_t possible[2] = { 0, 0 };

            for (uint8_t i = 0; i < 9; i++) {
                if (tab[cell][i]) {
                    possible[0]++;
                    possible[1] = i + 1;
                }
            }
            if (possible[0] == 1) {
                num_tab[cell] = possible[1];
            }
        }
    }

    return true;
}

bool Solver::isCorrect() {
    /*
    If there's any cell with no possible numbers, there's no solution
    */

    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t possible = 0;
            uint8_t cell = 9 * y + x;
            for (uint8_t i = 0; i < 9; i++) {
                if (tab[cell][i])
                    possible++;
            }
            if (possible == 0)
                return false;
        }
    }
    return true;
}

bool Solver::checkChanged(bool previous[81][9]) {
    /*
    Check whether there were any changes made since last iteration
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = y * 9 + x;
            for (uint8_t i = 0; i < 9; i++) {
                if (tab[cell][i] != previous[cell][i])
                    return true;
            }
        }
    }
    return false;
}

void Solver::findLowestPossibleCell(uint8_t coords[2]) {
    uint8_t lowestSoFar = 10;
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t possible = 0;
            for (uint8_t i = 0; i < 9; i++) {
                if (tab[9 * y + x][i])
                    possible++;
            }
            if (possible < lowestSoFar && possible != 1) {
                lowestSoFar = possible;
                coords[0] = y;
                coords[1] = x;
            }
        }
    }
}

void Solver::copyTable(bool fromTable[81][9], bool toTable[81][9]) {
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            for (uint8_t i = 0; i < 9; i++) {
                toTable[9 * y + x][i] = fromTable[9 * y + x][i];
            }
        }
    }
}

void Solver::copyTable(uint8_t fromTable[81], uint8_t toTable[81]) {
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            toTable[9 * y + x] = fromTable[9 * y + x];
        }
    }
}

void Solver::setNumber(uint8_t coords, uint8_t number) {
    for (uint8_t a = 0; a < 9; a++) {
        tab[coords][a] = 0;
    }
    tab[coords][number] = 1;
    num_tab[coords] = number + 1;
}

void Solver::getSquare(uint8_t* square, uint8_t id) {
    /*
    Get coords of all cells in a 3x3 square for easier
    checking and manipulation

    Squares IDs:
    0 1 2
    3 4 5
    6 7 8
    */
    uint8_t sq = 27 * floor(id / 3) + 3 * id % 3;

    square[0] = sq;
    square[1] = sq + 1;
    square[2] = sq + 2;
    square[3] = sq + 9;
    square[4] = sq + 10;
    square[5] = sq + 11;
    square[6] = sq + 18;
    square[7] = sq + 19;
    square[8] = sq + 20;
}

bool Solver::start() {
    if (num_tab[0] == 10) {
        printf("Nie zainicjalizowano tablicy.");
        return false;
    }

    firstFillPossible();        // Fill all empty cells

    if (solve())
        return true;
    else
        return false;
}

bool Solver::solve() {
    while (1) {
        bool prev_tab[81][9];
        copyTable(tab, prev_tab); // Copy table for later comparison
        elimRows();             // Each number can appear only once in a row,
        elimCols();             // a collumn
        elimSquares();          // or a 3x3 square

        onlyOneInRow();         // If there's only one space where the
        onlyOneInCol();         // number can fit in a row, a column or a square
        onlyOneInSquare();      // force it there

        if (!isCorrect()) { // fill numbers that are certain to the num_tab
            break;              // in case of mistake in solution exit
        }
        if (!checkChanged(prev_tab)) {  // check for changes
            /*
            If the algorithm couldn't go any further brute force a solution
            */
            uint8_t coords[2] = { 10, 10 };
            findLowestPossibleCell(coords);

            if (coords[0] == 10)    // Couldn't find >1 suggestion in any cell?
                return true;        // probably solved by this point

            bool copy_tab[81][9];   // Copy of both tabs before trying
            uint8_t copy_num_tab[81];  // any of the numbers
            copyTable(tab, copy_tab);
            copyTable(num_tab, copy_num_tab);

            uint8_t cell = 9 * coords[0] + coords[1];
            for (uint8_t i = 0; i < 9; i++) {
                if (tab[cell][i]) {
                    setNumber(cell, i);                     // Force a number into a cell
                    if (solve())                            // If it works
                        return true;                        // it works
                    else {                                  // if it doesn't
                        copyTable(copy_tab, tab);           // roll back changes
                        copyTable(copy_num_tab, num_tab);
                    }
                }
            }

        }
        copyTable(tab, prev_tab);
    }
    return false;
}

void Solver::printOutTable() {
    /*
    Print num_tab
    */
    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            printf("%d ", num_tab[y * 9 + x]);
            if (x % 3 == 2)
                printf("   ");
        }
        printf("\n");
        if (y % 3 == 2) {
            printf("\n");
        }
    }
}

void Solver::printOutPossibles() {
    /*
    Print tab
    */

    for (uint8_t y = 0; y < 9; y++) {
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = 9 * y + x;
            for (uint8_t i = 0; i < 3; i++) {
                uint8_t number = tab[cell][i] * (i + 1);
                printf("%d ", number);
            }
            printf("  ");
            if (x % 3 == 2)
                printf("   ");
        }
        printf("\n");
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = 9 * y + x;
            for (uint8_t i = 3; i < 6; i++) {
                uint8_t number = tab[cell][i] * (i + 1);
                printf("%d ", number);
            }
            printf("  ");
            if (x % 3 == 2)
                printf("   ");
        }
        printf("\n");
        for (uint8_t x = 0; x < 9; x++) {
            uint8_t cell = 9 * y + x;
            for (uint8_t i = 6; i < 9; i++) {
                uint8_t number = tab[cell][i] * (i + 1);
                printf("%d ", number);
            }
            printf("  ");
            if (x % 3 == 2)
                printf("   ");
        }
        printf("\n\n\n");
    }

}

Solver::Solver() {
    num_tab[0] = 10;
}

Solver::Solver(uint8_t* table) {
for (uint8_t y = 0; y < 9; y++) {
    for (uint8_t x = 0; x < 9; x++) {
        num_tab[y * 9 + x] = table[(y * 9) + x];
        for (uint8_t i = 0; i < 9; i++) {
            tab[y * 9 + x][i] = 0;
        }
        if (num_tab[y * 9 + x] != 0) {
            uint8_t number = num_tab[y * 9 + x] - 1;
            tab[y * 9 + x][number] = 1;
        }
    }
}

}