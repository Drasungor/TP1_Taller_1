#ifndef _SUDOKU_HANDLER_H_
#define  _SUDOKU_HANDLER_H_

#include "sudoku.h"

typedef struct{
  sudoku_t sudoku;
}sudoku_handler_t;



int sudoku_handler_init(sudoku_handler_t *sudoku_handler);

void sudoku_handler_release(sudoku_handler_t *sudoku_handler);

int sudoku_handler_set_number(sudoku_handler_t *sudoku_handler, int number, int vertical_position, int horizontal_position);

void sudoku_handler_reset(sudoku_handler_t *sudoku_handler);

bool sudoku_handler_verify(sudoku_handler_t *sudoku_handler);

void sudoku_handler_get_board(sudoku_handler_t *sudoku_handler, char buffer[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1]);



#endif
