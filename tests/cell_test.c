#include <stdio.h>
#include "../cell.h"
#include "cell_test.h"
#include "testing.h"

void unmodified_cell(){
  printf("TESTS UNMODIFIED CELL\n");

  cell_t cell;
  cell_init(&cell);
  print_test("Cell's number is initialized with 0", cell_get_number(&cell) == 0);
  print_test("Setting a number below 0 returns INVALID_NUMBER error", cell_set(&cell, -1) == INVALID_NUMBER);
  print_test("Setting a number above 9 returns INVALID_NUMBER error", cell_set(&cell, 10) == INVALID_NUMBER);
  print_test("Setting number 5 returns SUCCESS", cell_set(&cell, 5) == SUCCESS);
  print_test("Getting cell number returns 5", cell_get_number(&cell) == 5);
  printf("\n");
}

void modified_default_cell(){
  printf("TESTS MODIFIED CELL\n");

  cell_t cell;
  cell_init(&cell);
  print_test("Setting a default number below 0 returns INVALID_NUMBER error", !cell_set_as_default(&cell, -1));
  print_test("Setting a number above 9 returns INVALID_NUMBER error", !cell_set_as_default(&cell, 10));
  print_test("Setting default number 5 returns SUCCESS", cell_set_as_default(&cell, 5));
  print_test("Setting number 3 returns IS_DEFAULT", cell_set(&cell, 5) == IS_DEFAULT);
  printf("\n");
}

void modified_not_default_cell(){
  printf("TESTS MODIFIED NOT DEFAULT CELL\n");

  cell_t cell;
  cell_init(&cell);
  cell_set(&cell, 3);
  cell_set(&cell, 5);
  print_test("Setting number after setting previously saves the last number", cell_get_number(&cell) == 5);
  printf("\n");
}

void cell_test_run(){

  printf("CELL TESTS\n\n");
  unmodified_cell();
  modified_default_cell();
  modified_not_default_cell();
  printf("\n\n\n");
}
