#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#define BOARD_DIMENSION 9


typedef struct {
  cell_t board[BOARD_DIMENSION][BOARD_DIMENSION];
}sudoku_t;


//Initializes the game's beginning numbers
//If a number is below 0 (empty space) or above 9
//it fails and returns 1, otherwise returns 0
int sudoku_init(sudoku_t * sudoku, int initial_numbers[BOARD_DIMENSION][BOARD_DIMENSION]);


//Sets the cell in the position specified to the
//value written. It fails if a value is below 0 or
//above 9, the positions (from 0 to 8) are out of bounds
//or the value that is trying to be changed is one of
//the default non zero ones. Returns 1 on error and 0
//on success
int sudoku_set_number(sudoku_t * sudoku, int value, int vertical_position, int horizontal_position);

#endif
