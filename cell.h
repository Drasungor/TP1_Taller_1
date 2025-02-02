#ifndef _CELL_H_
#define _CELL_H_

#include <stdbool.h>

#define EMPTY_CELL_VALUE 0


//all attributes are private
typedef struct{
  int number;
  bool is_default;
}cell_t;


void cell_init(cell_t *cell);


//Sets the value of the cell and indicates it is one
//of the default values that the board begins with
void cell_set_as_default(cell_t *cell, int number);


//If the cell is default or the number is invalid it does nothing
//and returns an error value, otherwise sets copies the number in the cell
//and returns a success value
int cell_set(cell_t *cell, int number);


//Returns the number holded by the cell
int cell_get_number(const cell_t *cell);


void cell_release(cell_t *cell);


#endif
