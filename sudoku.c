#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"
#include "cell.h"

#define BLOCK_DIMENSION 3
#define CHARACTERS_BY_LINE 18

#define SUCCESS 0

typedef bool (*visit_t)(void*, void*);

//auxiliar struct to reduce the repeted values'
//checker firm lenght
//It determines a region in a matrix that goes from
//first to last-1 on each index
typedef struct{
  size_t first_i;
  size_t last_i;
  size_t first_j;
  size_t last_j;
}limits_t;



/*
static void iterate_matrix(void *matrix, limit_t limit, visit_t v, void* extra){
  for (size_t i = limit.first_i; i < limit.last_i+1; i++) {
    for (size_t i = limit.first_j; i < limit.last_j+1; i++) {
      if (!v()) {
      }
    }
  }
}
*/

void set_block_limits(limits_t *limits, size_t first_i, size_t first_j){
  limits->first_i = first_i;
  limits->last_i = first_i + BLOCK_DIMENSION;
  limits->first_j = first_j;
  limits->first_j = first_j + BLOCK_DIMENSION;
}


void set_row_limits(limits_t *limits, size_t i){
  limits->first_i = i;
  limits->last_i = i;
  limits->first_j = 0;
  limits->first_j = BOARD_DIMENSION;
}

void set_column_limits(limits_t *limits, size_t j){
  limits->first_i = 0;
  limits->last_i = BOARD_DIMENSION;
  limits->first_j = j;
  limits->first_j = j;
}

bool is_in_array(int array[], size_t data_size, int n){
  for (size_t i = 0; i < data_size; i++) {
    if (array[i] == n) {
      return true;
    }
  }
  return false;
}

void add_to_checker_array(int array[], size_t* data_size, int n){
  if (n != EMPTY_CELL_VALUE) {
    array[*data_size] = n;
    (*data_size)++;
  }
}

//checks if the area delimited has repeated values
//that go from 1 to 9
bool has_repeated_values(cell_t matrix[][BOARD_DIMENSION], limits_t limits){
  int found_numbers[BOARD_DIMENSION];
  size_t data_size = 0;
  int aux = 0;

  for (size_t i = limits.first_i; i < limits.last_i; i++) {
    for (size_t j = limits.first_j; j < limits.last_j; j++) {
      aux = cell_get_number(&matrix[i][j]);
      if (!is_in_array(found_numbers, data_size, aux)) {
        add_to_checker_array(found_numbers, &data_size, aux);
      } else {
        return true;
      }
    }
  }
  return false;
}

//ACHICAR LA FUNCION

//Receives de position of the top left number of the block and
//indicates if the block has every number from 1 to 9
/*
static bool verify_block(sudoku_t *sudoku, int vertical_index, int horizontal_index){
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
*/
/*
static bool verify_block(sudoku_t *sudoku, int i, int j){
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
*/

/*
static bool verify_blocks(sudoku_t *sudoku){
  for (size_t i = 0; i < BLOCK_DIMENSION; i++) {
    for (size_t j = 0; j < BLOCK_DIMENSION; j++) {
      if (!verify_block(sudoku, i+i * BLOCK_DIMENSION, j+j*BLOCK_DIMENSION)) {
        return false;
      }
    }
  }
  return true;
}
*/

static bool verify_blocks(cell_t board[][BOARD_DIMENSION]){
  limits_t limits;
  for (size_t i = 0; i < BLOCK_DIMENSION; i++) {
    for (size_t j = 0; j < BLOCK_DIMENSION; j++) {
      set_block_limits(&limits, i+i * BLOCK_DIMENSION, j+j*BLOCK_DIMENSION);
      if (has_repeated_values(board, limits)) {
        return false;
      }
    }
  }
  return true;
}


/*
static bool verify_row(sudoku_t *sudoku, int index){
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

static bool verify_rows(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    if (!verify_row(sudoku, i)) {
      return false;
    }
  }
  return true;
}
*/

static bool verify_rows(cell_t board[][BOARD_DIMENSION]){
  limits_t limits;
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    set_row_limits(&limits, i);
    if (has_repeated_values(board, limits)) {
      return false;
    }
  }
  return true;
}

//MODULARIZAR, ESTAS 3 FUNCIONES SE PUEDEN CONVERTIR EN UNA SOLA
//QUE RECIBA COMO PARAMETRO LA FUNCION PARA SETEAR LOS LIMITES
//PARA EL CASO DE LAS FILAS Y LAS COLUMNAS
static bool verify_columns(cell_t board[][BOARD_DIMENSION]){
  limits_t limits;
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    set_column_limits(&limits, i);
    if (has_repeated_values(board, limits)) {
      return false;
    }
  }
  return true;
}

/*
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
static bool verify_columns(sudoku_t *sudoku){
  for (size_t j = 0; j < BOARD_DIMENSION; j++) {
    if (!verify_column(sudoku, j)) {
      return false;
    }
  }
  return true;
}
*/

//Initializes the game's beginning numbers
//If a number is below 0 (empty space) or above 9

//CORREGIR ESTA ULTIMA LINEA YA QUE LA FUNCION NO HACE LO QUE DICE
//it fails and returns 1, otherwise returns 0
void sudoku_init(sudoku_t *sudoku, int initial_numbers[BOARD_DIMENSION][BOARD_DIMENSION]){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_init(&(sudoku->board[i][j]));
      cell_set_as_default(&(sudoku->board[i][j]), initial_numbers[i][j]);
    }
  }
}


//ADEMAS DE SACAR ESTA FUNCION DE ACA Y HACER UNA CLASE sudoku_parser
//VER SI CONVIENE RECIBIR EL ARCHIVO QUE SE VA A USAR PARA INICIALIZAR,
//CONVIENE PARA TESTING Y POR SI DESPUÉS SE QUIERE CAMBIAR EL ARCHIVO DEL
//QUE SE LEE
/*
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
*/
void sudoku_release(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_release(&(sudoku->board[i][j]));
    }
  }
}


//Sets the cell in the position specified to the
//value written. It fails if a value is below 0 or
//above 9, the positions (from 1 to 9) are out of bounds
//or the value that is trying to be changed is one of
//the default non zero ones. Returns 1 on error and 0
//on success
int sudoku_set_number(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position){
  return cell_set(&(sudoku->board[vertical_position-1][horizontal_position-1]), number);
}


//VER SI ESTA FUNCION ES INUTIL, XQ LAS DEFAULT SE INICIALIZAN AL
//INICIALIZAR EL SUDOKU, NO HAY X Q HACERLO UNA PRIMITIVA DE sudoku_t

//Works in the same way as sudoku_set_number but the
//number set is not erased when sudoku_reset is called
void sudoku_set_number_as_default(sudoku_t *sudoku, int number, int vertical_position, int horizontal_position){
  cell_set_as_default(&(sudoku->board[vertical_position-1][horizontal_position-1]), number);
}


//Sets all player set cells to 0
void sudoku_reset(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      //VER SI HACE FALTA USAR UN .h DE CONSTANTES PARA QUE
      //EL FUNCIONAMIENTO NO DEPENDA DE EMPTY_CELL_VALUE
      //PUEDE SER QUE ESTO NO HAGA FALTA
      cell_set(&(sudoku->board[i][j]), EMPTY_CELL_VALUE);
    }
  }
}

//Indicates if the current state of the board follows
//the rules of the game
bool sudoku_verify(sudoku_t *sudoku){
  if (!verify_blocks(sudoku->board)) {
    return false;
  }
  if (!verify_rows(sudoku->board)) {
    return false;
  }
  return  verify_columns(sudoku->board);
}

/*
void sudoku_get_board(sudoku_t *sudoku, int destiny[BOARD_DIMENSION][BOARD_DIMENSION]){
  char board[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD];
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      destiny[i][j] = cell_get_number(&(sudoku->board[i][j]));
    }
  }
}
*/

static char int_to_char(int n){
  if (n == EMPTY_CELL_VALUE) {
    return ' ';
  }
  return n+48;
}


//FALTA AGREGAR EL CASO DE LOS '+'
//PASAR TODOS LOS NROS A CTES: SEPARACION_ENTRE '|' POR EJ
static char select_char(size_t i, size_t j, int number){
  if (j == HORIZONTAL_DIM_PRINTED_BOARD) {
    return '\n';
  }
  if (j % 12 == 0) {
    return 'U';
  }
  if (i % 6 == 0) {
    return '=';
  }
  if ((i % 2 == 0) && (j % 4 == 0)) {
    return '+';
  }
  if (i % 2 == 0) {
    return '-';
  }
  if (j % 4 == 0) {
    return '|';
  }
  //return int_to_char(number);
  //CAMBIAR EL ESPACIO POR EMPTI_CELL_CHAR O ALGO ASI
  return ' ';
}

//VER SI SE PUEDE CAMBIAR POR UNA UNICA FUNCION Q HAGA ESTO EN VEZ
//DE TENES DOS FOR DISTINTOS
void sudoku_get_board(sudoku_t *sudoku, char buffer[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1]){
  int number = 0;
  for (size_t i = 0; i < VERTICAL_DIM_PRINTED_BOARD; i++) {
    for (size_t j = 0; j < HORIZONTAL_DIM_PRINTED_BOARD + 1; j++) {
      //int number = cell_get_number(&(sudoku->board[i][j]));
      //BORRAR, ES PARA DEBUGGING
      //printf("%c, %d\n", select_char(i, j, number), number);
      buffer[i][j] = select_char(i, j, number);
    }
  }
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      number = cell_get_number(&(sudoku->board[i][j]));
      buffer[1 + i * 2][2 + j * 4] = int_to_char(number);
    }
  }
}
