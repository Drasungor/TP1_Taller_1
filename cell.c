#include "cell.h"


bool is_valid_number(int number){
  return (number >= 0) && (number <= 9);
}





void cell_init(cell_t *cell){
  cell->number = 0;
  cell->is_default = false;
}


bool cell_set_as_default(cell_t *cell, int number){
  if (!is_valid_number(number)) {
    return false;
  }

  //VER SI DEBERIA SACAR ESTE IF
  if (number != EMPTY_CELL_VALUE) {
    cell->is_default = true;
  }
  return true;
}


int cell_set(cell_t *cell, int number){
  if (!is_valid_number(number)) {
    return INVALID_NUMBER;
  }
  if (cell->is_default) {
    return IS_DEFAULT;
  }
  cell->number = number;
  return SUCCESS;
}
