#include "cell.h"


#define SUCCESS 0
#define IS_DEFAULT -1


void cell_init(cell_t *cell){
  cell->number = 0;
  cell->is_default = false;
}


void cell_set_as_default(cell_t *cell, int number){
  if (number != EMPTY_CELL_VALUE) {
    cell->is_default = true;
  }
  cell->number = number;
}


int cell_set(cell_t *cell, int number){
  if (cell->is_default) {
    return IS_DEFAULT;
  }
  cell->number = number;
  return SUCCESS;
}


int cell_get_number(const cell_t *cell){
  return cell->number;
}


void cell_release(cell_t *cell){
  //DO NOTHING
}
