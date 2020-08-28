#include <string>
#include <fstream>
#include "solver.h"

using namespace std;

int main(){
    
uint8_t sudotab[81];
fstream file;

file.open("sudoku.txt", ios::in);

if (file.is_open()) {
    string line;
    uint8_t tab_index = 0;
    while (getline(file, line)) {
        for (uint8_t x = 0; x < 9; x++) {
            char c = line[x];
            uint8_t number = c - 48;
            if (number >= 0 || number <= 9)
                sudotab[tab_index] = number;
            else {
                printf("Error reading sudoku table. Exiting program.");
                return 0;
            }
            tab_index++;
        }
    }
    file.close();
}
else {
    printf("No file\n");
}

Solver solv(sudotab);
solv.printOutTable();
printf("\n");
if (solv.start()) {
    printf("Solved\n");
}
else {
    printf("Can't solve\n");
}
solv.printOutTable();

}