#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_handler.h"

#define SUCCESS 0
#define INVALID_NUMBER -1
#define OUT_OF_BOUNDS -2
#define FILE_ERROR -3
#define MEMORY_ERROR -4

static int char_to_int(char c){
  return c - 48;
}

int sudoku_handler_init(sudoku_handler_t *sudoku_handler){
  FILE *sudoku_file =  fopen("sudoku.txt", "r");
  if (!sudoku_file) {
    return FILE_ERROR;
  }

  char *line;
  size_t size;
  //char num[2];
  //num[1] = '\0';
  int number = 0;
  int board [BOARD_DIMENSION][BOARD_DIMENSION];

  //MODULARIZAR MUCHO

  //VER SI HAY QUE AGREGAR CHEQUEOS DE VALORES USADOS
  //PARA INICIALIZAR
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    //VER SI SE PUEDE SOLUCIONAR ESTE COMENTARIO DE ABAJO
    //fgets throws error for unused return value
    line = NULL;
    size = 0;
    //HACER CHEQUEO POR SI FALLA GETLINE
    size = getline(&line, &size, sudoku_file);
    if (size == -1) {
      return MEMORY_ERROR;
    }
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      number = line[2*j];
      //ESTOY USANDO CHARS, VER SI CONVIENE QUE SE GUARDEN CHARS EN VEZ DE INTS
      //SI NO SE PASA ACA DE CHAR A INT ENTONCES SE VA A TENER QUE PASAR DESPUÉS DE
      //INT A CHAR LOS NROS QUE LLEGUEN DEL CLIENTE, PERO DESPUÉS SE AHORRA EL TRABAJO
      //DE PASAR TODOS LOS CARACTERES A CHAR PARA DEVOLVER LA MATRIZ PARA IMPRIMIR
      //sudoku_set_number_as_default(sudoku_handler->sudoku, char_to_int(number), i, j);
      board[i][j] = char_to_int(number);
    }
    free(line);
  }
  fclose(sudoku_file);
  sudoku_init(&(sudoku_handler->sudoku), board);
  return SUCCESS;
}

void sudoku_handler_release(sudoku_handler_t *sudoku_handler){
  sudoku_release(&(sudoku_handler->sudoku));
}

//CAMBIAR i y j POR NOMBRES MAS CLAROS, ACA Y EN sudoku_t
int sudoku_handler_set_number(sudoku_handler_t *sudoku_handler, int number, int i, int j){
  return sudoku_set_number(&(sudoku_handler->sudoku), number, i, j);
}

void sudoku_handler_reset(sudoku_handler_t *sudoku_handler){
  sudoku_reset(&(sudoku_handler->sudoku));
}

bool sudoku_handler_verify(sudoku_handler_t *sudoku_handler){
  return sudoku_verify(&(sudoku_handler->sudoku));
}

void sudoku_handler_get_board(sudoku_handler_t *sudoku_handler, char buffer[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1]){
  sudoku_get_board(&(sudoku_handler->sudoku), buffer);


}
