#ifndef _SUDOKU_HANDLER_H_
#define  _SUDOKU_HANDLER_H_

#include "sudoku.h"

typedef struct{
  sudoku_t sudoku;
}sudoku_handler_t;



int sudoku_handler_init(sudoku_handler_t *sudoku_handler);

void sudoku_handler_release(sudoku_handler_t *sudoku_handler);

int sudoku_handler_set_number(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position);

void sudoku_handler_reset(sudoku_t *sudoku);

bool sudoku_handler_verify(sudoku_t *sudoku);

void sudoku_handler_get_board(sudoku_t *sudoku, int destination[BOARD_DIMENSION][BOARD_DIMENSION]);



#endif
