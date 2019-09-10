#ifndef _SUDOKU_HANDLER_H_
#define  _SUDOKU_HANDLER_H_

#include "sudoku.h"

//all attributes are private
typedef struct{
  sudoku_t sudoku;
}sudoku_handler_t;


//Initializes the sudoku reading the file "board.txt"
//It returns 0 if it's successful, -1 if there is a file
//error, and -2 if there is a memory error
int sudoku_handler_init(sudoku_handler_t *sudoku_handler);


void sudoku_handler_release(sudoku_handler_t *sudoku_handler);

//Tries to set the number in the coordinates given,
//if the position belongs to a nuber set in initialization
//returns -1, otherwise returns 0
int sudoku_handler_set_number(sudoku_handler_t *sudoku_handler,
                              int number,
                              int vertical_position,
                              int horizontal_position);


void sudoku_handler_reset(sudoku_handler_t *sudoku_handler);


//Returns a boolean that indicates if the current state of the
//sudoku meets the requirements of a sudoku in a correct state
//without taking into account the empty values
//If its ok returns true, otherwise returns false
bool sudoku_handler_verify(sudoku_handler_t *sudoku_handler);


//Copies in the buffer the current state of the board, adding
//\n in all the elements with j = HORIZONTAL_DIM_PRINTED_BOARD
void sudoku_handler_get_board(sudoku_handler_t *sudoku_handler,
                              char buffer[VERTICAL_DIM_PRINTED_BOARD]
                                         [HORIZONTAL_DIM_PRINTED_BOARD + 1]);

#endif
