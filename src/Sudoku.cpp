#include "Sudoku.h"

Sudoku::Sudoku()
{
    for (int i=0; i<9 ; i++){
        for (int j=0; j<9; j++){
            values[i][j] = 0;
        }
    }
    std::fill(&possibilities[0][0][0],&possibilities[0][0][0]+sizeof(possibilities),true);
}

Sudoku::~Sudoku()
{
    //dtor
}
