#ifndef _SUDOKU_H_
#define _SUDOKU_H_

#include <stdbool.h>
#include "cell.h"

#define BOARD_DIMENSION 9
#define HORIZONTAL_DIM_PRINTED_BOARD 37
#define VERTICAL_DIM_PRINTED_BOARD 19


typedef struct {
  cell_t board[BOARD_DIMENSION][BOARD_DIMENSION];
}sudoku_t;


//Initializes the game's beginning numbers
//If a number is below 0 (empty space) or above 9
//it fails and returns 1, otherwise returns 0
void sudoku_init(sudoku_t *sudoku, int initial_numbers[BOARD_DIMENSION][BOARD_DIMENSION]);

int sudoku_init_with_file(sudoku_t *sudoku);

void sudoku_release(sudoku_t *sudoku);

//Sets the cell in the position specified to the
//value written. It fails if a value is below 0 or
//above 9, the positions (from 0 to 8) are out of bounds
//or the value that is trying to be changed is one of
//the default non zero ones. Returns 1 on error and 0
//on success
int sudoku_set_number(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position);

//Works in the same way as sudoku_set_number but the
//number set is not erased when sudoku_reset is called
void sudoku_set_number_as_default(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position);

//Sets all player set cells to 0
void sudoku_reset(sudoku_t *sudoku);

//Indicates if the current state of the board follows
//the rules of the game
bool sudoku_verify(sudoku_t *sudoku);

//Copies the current state of the sudoku in the matrix

//Copies the current state of the board in destiny
//VER SI CONVIENE DEVOLVER UN PUNTERO CON MEMORIA DINAMICA
void sudoku_get_board(sudoku_t *sudoku, char buffer[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1]);

#endif
