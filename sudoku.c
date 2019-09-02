#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include "cell.h"

#define BLOCK_DIMENSION 3
#define CHARACTERS_BY_LINE 18

//PASAR LOS NUMEROS A CONSTANTES
#define IS_BOUNDED(x) ((x>=1) && (x<=9))

bool is_in_bounds(int vertical_position, int horizontal_position){
  return (IS_BOUNDED(vertical_position)) && (IS_BOUNDED(horizontal_position));
}


//ACHICAR LA FUNCION

//Receives de position of the top left number of the block and
//indicates if the block has every number from 1 to 9
bool verify_block(sudoku_t *sudoku, int vertical_index, int horizontal_index){
  //Each number has an index of number-1
  bool is_number_in_block[BOARD_DIMENSION + 1];
  int number = 0;

  //sets all values of the array to false
  memset(is_number_in_block, 0, BOARD_DIMENSION * sizeof(bool));

  for (size_t i = vertical_index; i < vertical_index + BLOCK_DIMENSION; i++) {
    for (size_t j = horizontal_index; j < horizontal_index + BLOCK_DIMENSION; j++) {
      number = cell_get_number(&(sudoku->board[i][j]));

      //VER SI ES MEJOR CHEQUEAR ACA SI ES 0 PORQUE YA PUEDO SALIR DE LA
      //ITERACION
      if (!is_number_in_block[number]) {
        is_number_in_block[number] = true;
      } else{
        return false;
      }
    }
  }


  //PASARLO A FUNCION A PARTE
  if (is_number_in_block[0]) {
    return false;
  }
  for (size_t i = 1; i < BOARD_DIMENSION+1; i++) {
    if (!is_number_in_block[i]) {
      return false;
    }
  }

  return true;

}

bool verify_blocks(sudoku_t *sudoku){
  for (size_t i = 0; i < BLOCK_DIMENSION; i++) {
    for (size_t j = 0; j < BLOCK_DIMENSION; j++) {
      if (!verify_block(sudoku, i+i * BLOCK_DIMENSION, j+j*BLOCK_DIMENSION)) {
        return false;
      }
    }
  }
  return true;
}


bool verify_row(sudoku_t *sudoku, int index){
  bool is_number_in_row[BOARD_DIMENSION + 1];
  int number = 0;

  //sets all values of the array to false
  memset(is_number_in_row, 0, BOARD_DIMENSION * sizeof(bool));

  for (size_t j = 0; j < BOARD_DIMENSION; j++) {
    number = cell_get_number(&(sudoku->board[index][j]));

    //VER SI ES MEJOR CHEQUEAR ACA SI ES 0 PORQUE YA PUEDO SALIR DE LA
    //ITERACION
    if (!is_number_in_row[number]) {
      is_number_in_row[number] = true;
    } else{
      return false;
    }
  }

  //PASARLO A FUNCION A PARTE
  if (is_number_in_row[0]) {
    return false;
  }

  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    if (!is_number_in_row[i]) {
      return false;
    }
  }

  return true;
}

bool verify_rows(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    if (!verify_row(sudoku, i)) {
      return false;
    }
  }
  return true;
}

bool verify_column(sudoku_t *sudoku, int index){
  bool is_number_in_column[BOARD_DIMENSION + 1];
  int number = 0;

  //sets all values of the array to false
  memset(is_number_in_column, 0, BOARD_DIMENSION * sizeof(bool));

  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    number = cell_get_number(&(sudoku->board[i][index]));

    //VER SI ES MEJOR CHEQUEAR ACA SI ES 0 PORQUE YA PUEDO SALIR DE LA
    //ITERACION
    if (!is_number_in_column[number]) {
      is_number_in_column[number] = true;
    } else{
      return false;
    }
  }

  //PASARLO A FUNCION A PARTE
  if (is_number_in_column[0]) {
    return false;
  }

  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    if (!is_number_in_column[i]) {
      return false;
    }
  }

  return true;
}


//VER SI SE PUEDE MODULARIZAR CON veify_rows()
bool verify_columns(sudoku_t *sudoku){
  for (size_t j = 0; j < BOARD_DIMENSION; j++) {
    if (!verify_column(sudoku, j)) {
      return false;
    }
  }
  return true;
}


//Initializes the game's beginning numbers
//If a number is below 0 (empty space) or above 9

//CORREGIR ESTA ULTIMA LINEA YA QUE LA FUNCION NO HACE LO QUE DICE
//it fails and returns 1, otherwise returns 0
int sudoku_init(sudoku_t *sudoku, int initial_numbers[BOARD_DIMENSION][BOARD_DIMENSION]){



  //VER SI SE PUEDE CAMBIAR LA ITERACION POR UNA MACRO PARA NO REPETIRLA
  //EN OTRAS PARTES DEL CODIGO
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_init(&(sudoku->board[i][j]));
      if (!cell_set_as_default(&(sudoku->board[i][j]), initial_numbers[i][j])) {
        //VER SI HAY QUE HACER UN ARCHIVO DE CONSTANTES PARA USAR EN TODO EL TP
        //O SI HAY QUE DEJAR LAS CONSTANTES EN LOS .c
        return INVALID_NUMBER;
      }
    }
  }
  return SUCCESS;
}



int sudoku_init_with_file(sudoku_t *sudoku){

  FILE *sudoku_file =  fopen("sudoku.txt", "r");
  if (!sudoku_file) {
    return FILE_ERROR;
  }

  char *line;
  size_t size;
  char num[2];
  num[1] = '\0';


  //VER SI HAY QUE AGREGAR CHEQUEOS DE VALORES USADOS
  //PARA INICIALIZAR
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    //VER SI SE PUEDE SOLUCIONAR ESTE COMENTARIO DE ABAJO
    //fgets throws error for unused return value
    line = NULL;
    size = 0;
    size = getline(&line, &size, sudoku_file);
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      num[0] = line[2*j];
      cell_set_as_default(&(sudoku->board[i][j]), atoi(num));
    }
    free(line);
  }

  fclose(sudoku_file);

  //VER SI NO TIENE SENTIDO QUE RETORNE ALGO, TAL VEZ SE
  //CAMBIE POR VOID
  return SUCCESS;

}

void sudoku_release(sudoku_t *sudoku){
  //HAY QUE ITERAR TODO EL BOARD PARA LIBERAR TODODAS LAS CELDAS
}


//Sets the cell in the position specified to the
//value written. It fails if a value is below 0 or
//above 9, the positions (from 1 to 9) are out of bounds
//or the value that is trying to be changed is one of
//the default non zero ones. Returns 1 on error and 0
//on success
int sudoku_set_number(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position){
  if (!is_in_bounds(vertical_position, horizontal_position)) {
    return OUT_OF_BOUNDS;
  }
  return cell_set(&(sudoku->board[vertical_position-1][horizontal_position-1]), number);
}

//Sets all player set cells to 0
void sudoku_reset(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_set(&(sudoku->board[i][j]), EMPTY_CELL_VALUE);
    }
  }
}

//Indicates if the current state of the board follows
//the rules of the game
bool sudoku_verify(sudoku_t *sudoku){
  return verify_blocks(sudoku) && verify_rows(sudoku) && verify_columns(sudoku);
}


void sudoku_get_board(sudoku_t *sudoku, int destiny[BOARD_DIMENSION][BOARD_DIMENSION]){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      destiny[i][j] = cell_get_number(&(sudoku->board[i][j]));
    }
  }
}
