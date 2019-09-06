#include <stdio.h>
#include "../sudoku_handler.h"
#include "sudoku_handler_test.h"
#include "testing.h"

void sudoku_handler_inicialization_test(){
  printf("SUDOKU HANDLER INICIALIZATION\n");

  sudoku_handler_t sudoku_handler;
  sudoku_handler_init(&sudoku_handler);
  char buffer[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1];
  sudoku_handler_get_board(&sudoku_handler, buffer);
  for (size_t i = 0; i < VERTICAL_DIM_PRINTED_BOARD; i++) {
    for (size_t j = 0; j < HORIZONTAL_DIM_PRINTED_BOARD + 1; j++) {
      printf("%c", buffer[i][j]);
    }
  }
}

void sudoku_handler_test_run(){
  printf("SUDOKU HANDLER TESTS\n\n");
  sudoku_handler_inicialization_test();
  printf("\n\n\n");
}
