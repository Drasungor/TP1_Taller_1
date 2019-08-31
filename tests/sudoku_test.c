#include <stdio.h>
#include <string.h>
#include "../sudoku.h"
#include "sudoku_test.h"
#include "testing.h"


void test_sudoku(){
  printf("SUDOKU TEST\n");

  int matrix[9][9];
  //initilices matrix elements to 0
  memset(matrix, 0, 9*9*sizeof(int));

  print_test("ESCRIBI PRUEBAS PARA SUDOKU FLACO", false);
}

void sudoku_test_run(){
  test_sudoku();
}
