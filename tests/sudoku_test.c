#include <stdio.h>
#include <string.h>
#include "../sudoku.h"
#include "sudoku_test.h"
#include "testing.h"

#define BOARD_DIMENSION 9

void test_sudoku(){
  printf("SUDOKU TEST\n");

  int matrix[BOARD_DIMENSION][BOARD_DIMENSION];
  //initilices matrix elements to 0
  memset(matrix, 0, BOARD_DIMENSION*BOARD_DIMENSION*sizeof(int));

  print_test("ESCRIBI PRUEBAS PARA SUDOKU FLACO", false);
}

void sudoku_test_run(){
  test_sudoku();
}
