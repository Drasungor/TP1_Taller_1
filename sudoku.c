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


static void _set_block_limits(limits_t *limits, size_t first_i, size_t first_j){
  limits->first_i = first_i;
  limits->last_i = first_i + BLOCK_DIMENSION;
  limits->first_j = first_j;
  limits->last_j = first_j + BLOCK_DIMENSION;
}


static void _set_row_limits(limits_t *limits, size_t i){
  limits->first_i = i;
  limits->last_i = i + 1;
  limits->first_j = 0;
  limits->last_j = BOARD_DIMENSION;
}

static void _set_column_limits(limits_t *limits, size_t j){
  limits->first_i = 0;
  limits->last_i = BOARD_DIMENSION;
  limits->first_j = j;
  limits->last_j = j + 1;
}

static bool _is_in_array(int array[], size_t data_size, int n){
  for (size_t i = 0; i < data_size; i++) {
    if (array[i] == n) {
      return true;
    }
  }
  return false;
}

static void _add_to_checker_array(int array[], size_t* data_size, int n){
  if (n != EMPTY_CELL_VALUE) {
    array[*data_size] = n;
    (*data_size)++;
  }
}

//checks if the area delimited has repeated values
//that go from 1 to 9
static bool _has_repeated_values(const cell_t matrix[BOARD_DIMENSION]
                                                    [BOARD_DIMENSION],
                                 limits_t limits){
  int found_numbers[BOARD_DIMENSION];
  size_t data_size = 0;
  int aux = 0;

  for (size_t i = limits.first_i; i < limits.last_i; i++) {
    for (size_t j = limits.first_j; j < limits.last_j; j++) {
      aux = cell_get_number(&matrix[i][j]);
      if (!_is_in_array(found_numbers, data_size, aux)) {
        _add_to_checker_array(found_numbers, &data_size, aux);
      } else {
        return true;
      }
    }
  }
  return false;
}


static bool _verify_blocks(const cell_t board[BOARD_DIMENSION]
                                             [BOARD_DIMENSION]){
  limits_t limits;
  for (size_t i = 0; i < BLOCK_DIMENSION; i++) {
    for (size_t j = 0; j < BLOCK_DIMENSION; j++) {
      _set_block_limits(&limits, i * BLOCK_DIMENSION, j*BLOCK_DIMENSION);
      if (_has_repeated_values(board, limits)) {
        return false;
      }
    }
  }
  return true;
}


static bool _verify_rows(const cell_t board[BOARD_DIMENSION][BOARD_DIMENSION]){
  limits_t limits;
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    _set_row_limits(&limits, i);
    if (_has_repeated_values(board, limits)) {
      return false;
    }
  }
  return true;
}

static bool _verify_columns(const cell_t board[BOARD_DIMENSION]
                                              [BOARD_DIMENSION]){
  limits_t limits;
  for (size_t j = 0; j < BOARD_DIMENSION; j++) {
    _set_column_limits(&limits, j);
    if (_has_repeated_values(board, limits)) {
      return false;
    }
  }
  return true;
}



static char _int_to_char(int n){
  if (n == EMPTY_CELL_VALUE) {
    return ' ';
  }
  return n+48;
}


//This function can't be reduced to 15 lines because it's necessary
//to have an inf for each character that has to be present in the
//matrix
//This function returns the character that has to be put in
//the matrix buffer initialized by the sudoku based in the
//position (i,j)
static char _select_char(size_t i, size_t j){
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
  return ' ';
}

//Initializes de matrix with the characters thar establish
//a separation between the numbers in the sudoku that is
//going to be printed
static void _set_delimiters(char buffer[VERTICAL_DIM_PRINTED_BOARD]
                                       [HORIZONTAL_DIM_PRINTED_BOARD + 1]){
  for (size_t i = 0; i < VERTICAL_DIM_PRINTED_BOARD; i++) {
    for (size_t j = 0; j < HORIZONTAL_DIM_PRINTED_BOARD + 1; j++) {
      buffer[i][j] = _select_char(i, j);
    }
  }
}

static void _set_numbers(const cell_t board[BOARD_DIMENSION][BOARD_DIMENSION],
                         char buffer[VERTICAL_DIM_PRINTED_BOARD]
                                    [HORIZONTAL_DIM_PRINTED_BOARD + 1]){
  int number = 0;
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      number = cell_get_number(&(board[i][j]));
      buffer[1 + i * 2][2 + j * 4] = _int_to_char(number);
    }
  }
}


void sudoku_init(sudoku_t *sudoku, int initial_numbers[BOARD_DIMENSION]
                                                      [BOARD_DIMENSION]){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_init(&(sudoku->board[i][j]));
      cell_set_as_default(&(sudoku->board[i][j]), initial_numbers[i][j]);
    }
  }
}


void sudoku_release(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_release(&(sudoku->board[i][j]));
    }
  }
}


int sudoku_set_number(sudoku_t *sudoku,
                      int number,
                      int vertical_position,
                      int horizontal_position){
  return cell_set(&(sudoku->board[vertical_position-1][horizontal_position-1]),
                  number);
}


void sudoku_reset(sudoku_t *sudoku){
  for (size_t i = 0; i < BOARD_DIMENSION; i++) {
    for (size_t j = 0; j < BOARD_DIMENSION; j++) {
      cell_set(&(sudoku->board[i][j]), EMPTY_CELL_VALUE);
    }
  }
}


bool sudoku_verify(const sudoku_t *sudoku){
  if (!_verify_blocks(sudoku->board)) {
    return false;
  }
  if (!_verify_rows(sudoku->board)) {
    return false;
  }
  if (!_verify_columns(sudoku->board)) {
    return false;
  }
  return  true;
}


void sudoku_get_board(const sudoku_t *sudoku,
                      char buffer[VERTICAL_DIM_PRINTED_BOARD]
                                 [HORIZONTAL_DIM_PRINTED_BOARD + 1]){
  _set_delimiters(buffer);
  _set_numbers(sudoku->board, buffer);
}
