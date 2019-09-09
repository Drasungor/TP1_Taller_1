#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "sudoku_handler.h"

#define BOARD_FILE_NAME "board.txt"

#define SUCCESS 0
#define INVALID_NUMBER -1
#define OUT_OF_BOUNDS -2
#define FILE_ERROR -3
#define MEMORY_ERROR -4

static int char_to_int(char c){
  return c - 48;
}



//This function can't be reduced to 15 lines due to variable
//initialization and error checking
static int board_file_to_matrix(FILE *file, int matrix[BOARD_DIMENSION][BOARD_DIMENSION]){
  char *line;
  size_t size;
  int number = 0;

  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    line = NULL;
    size = 0;
    size = getline(&line, &size, sudoku_file);
    if (size == -1) {
      free(line);
      return MEMORY_ERROR;
    }
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      number = line[2*j];
      board[i][j] = char_to_int(number);
    }
    free(line);
  }
  return SUCCESS;
}


//ASEGURARSE QUE DONDE SE LLAMA A sudoku_handler_init SE CHEQUEA EL VALOR DE RETORNO
int sudoku_handler_init(sudoku_handler_t *sudoku_handler){
  FILE *board_file =  fopen(BOARD_FILE_NAME, "r");
  if (!board_file) {
    return FILE_ERROR;
  }

  int board [BOARD_DIMENSION][BOARD_DIMENSION];
  int program_state = board_file_to_matrix(board_file, board);

  fclose(board_file);
  if (program_state != SUCCESS) {
    return program_state;
  }
  //MODULARIZAR MUCHO
  /*
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    line = NULL;
    size = 0;
    size = getline(&line, &size, sudoku_file);
    if (size == -1) {
      free(line);
      return MEMORY_ERROR;
    }
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      number = line[2*j];
      board[i][j] = char_to_int(number);
    }
    free(line);
  }
  */
  //fclose(sudoku_file);
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
