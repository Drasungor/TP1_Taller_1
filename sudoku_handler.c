#include "sudoku_handler.h"

#define INVALID_NUMBER -1
#define OUT_OF_BOUNDS -2
#define FILE_ERROR -3


int sudoku_handler_init(sudoku_handler_t *sudoku_handler){
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

void sudoku_handler_release(sudoku_handler_t *sudoku_handler){
}


int sudoku_handler_set_number(sudoku_t *sudoku, int number, int i, int j){
}

void sudoku_handler_reset(sudoku_t *sudoku){
}

bool sudoku_handler_verify(sudoku_t *sudoku){
}

void sudoku_handler_get_board(sudoku_t *sudoku, int destination[BOARD_DIMENSION][BOARD_DIMENSION]){
}
