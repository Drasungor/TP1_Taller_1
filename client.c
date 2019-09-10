#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"
#include "client_server_constants.h"


#define EXIT_INDICATOR 'E'

#define GET_COMMAND "get"
#define VERIFY_COMMAND "verify"
#define RESET_COMMAND "reset"
#define EXIT_COMMAND "exit"
#define PUT_COMMAND_FORMAT "put %d in %d,%d"
#define NUMBER_OF_INPUTS_PUT 3
#define MAX_RECEIVABLE_LENGHT 722
#define NUMBER_INDEX 0
#define VER_POS_INDEX 1
#define HOR_POS_INDEX 2

#define SUCCESS 0
#define SOCKET_ERROR -1
#define CLOSED_SOCKET -2
#define INVALID_COMMAND -3
#define INVALID_NUMBER -4
#define INVALID_COORDINATES -5
#define EXIT_PROGRAM -6
#define MEMORY_ERROR -7
#define END_OF_FILE -8
#define NOT_SIMPLE_COMMAND -9
#define FINISHED_IN_ERROR 1

//If there is an error it prints a message that describes it
static void _print_error(int program_status){
  if (program_status == SOCKET_ERROR) {
    fprintf(stderr, "Error de conexión\n");
  } else if (program_status == INVALID_NUMBER) {
    fprintf(stderr, "Error en el valor ingresado. Rango soportado: [1,9]\n");
  } else if (program_status == INVALID_COORDINATES) {
    fprintf(stderr, "Error en los índices. Rango soportado: [1,9]\n");
  } else if (program_status == MEMORY_ERROR) {
    fprintf(stderr, "Error en la asignación de memoria dinámica\n");
  }
}

static bool _strings_are_equal(char *command, char *input, size_t size){
  if (strlen(command) != size) {
    return false;
  }
  if (strncmp(command, input, size) != 0) {
    return false;
  }
  return true;
}


static bool _is_program_terminanting_error(int program_status){
  return (program_status == SOCKET_ERROR) || (program_status == MEMORY_ERROR);
}

static bool _is_program_terminanting_value(int program_status){
  if (program_status == EXIT_PROGRAM) {
    return true;
  }
  if (program_status == END_OF_FILE) {
    return true;
  }
  if (program_status == CLOSED_SOCKET) {
    return true;
  }
  return false;
}

static bool _should_kill_program(int program_status){
  return _is_program_terminanting_value(program_status) ||
         _is_program_terminanting_error(program_status);
}


static bool _is_valid_position(int position){
  return (position >= 1) && (position <= 9);
}


static bool _is_valid_number(int n){
  return (n >= 1) && (n <= 9);
}

//Indicates if the input is a valid format for the command put
//and puts in data the information that is goint to be sent to
//the server
static int _put_command_validation(char *input, size_t size, uint8_t data[3]){
  int number;
  int vertical_position;
  int horizontal_position;

  int values_read = sscanf(input,
                           PUT_COMMAND_FORMAT,
                           &number,
                           &vertical_position,
                           &horizontal_position);
  if ((values_read < NUMBER_OF_INPUTS_PUT) || (values_read == EOF)) {
    return INVALID_COMMAND;
  }
  if (!_is_valid_number(number)) {
    return INVALID_NUMBER;
  }
  if (!(_is_valid_position(vertical_position) &&
        _is_valid_position(horizontal_position))) {
    return INVALID_COORDINATES;
  }
  data[NUMBER_INDEX] = number;
  data[VER_POS_INDEX] = vertical_position;
  data[HOR_POS_INDEX] = horizontal_position;
  return SUCCESS;
}


static void _print_message(char* message, size_t size){
  message[size] = '\0';
  printf("%s", message);
}

//Prints the message that is received
static int _print_answer(socket_t *sckt){
  uint32_t message_size;
  int program_status = socket_receive(sckt, &message_size, sizeof(uint32_t));
  if (program_status != SUCCESS) {
    return program_status;
  }
  message_size = ntohl(message_size);
  char message[MAX_RECEIVABLE_LENGHT + 1];
  program_status = socket_receive(sckt, message, message_size);
  if (program_status != SUCCESS) {
    return program_status;
  }
  _print_message(message, message_size);
  return SUCCESS;
}

//Excecutes the processes necessary to obtain the answer to a
//command that only needs to send it's indicator to be executed
static int _obtain_answer_simple(socket_t *sckt, char indicator){
  char indicator_copy = indicator;
  int program_status = socket_send(sckt, &indicator_copy, sizeof(char));
  if (program_status != SUCCESS) {
    return program_status;
  }
  return _print_answer(sckt);
}


//Tries to send a petition for the execution of the command put,
//that puts a number in a position in the sudoku stored in the server
//If the input is invalid then exits with the correspondent error value
static int _obtain_answer_for_put(socket_t *sckt,
                                  char *input,
                                  size_t input_size){
  uint8_t data[3];
  int put_validation = _put_command_validation(input, input_size, data);
  if (put_validation != SUCCESS) {
    return put_validation;
  }
  char indicator = PUT_INDICATOR;
  int program_status = socket_send(sckt, &indicator, sizeof(char));
  if (program_status != SUCCESS) {
    return program_status;
  }
  program_status = socket_send(sckt, data, 3 * sizeof(uint8_t));
  if (program_status != SUCCESS) {
    return program_status;
  }
  return _print_answer(sckt);
}


//Returns the indicator associated to the command given
//If the command does not exist ir returns INVALID_COMMAND
static char _get_command_indicator(char *input, size_t size){
  if (_strings_are_equal(VERIFY_COMMAND, input, size)) {
    return VERIFY_INDICATOR;
  } else if (_strings_are_equal(RESET_COMMAND, input, size)) {
    return RESET_INDICATOR;
  } else if (_strings_are_equal(GET_COMMAND, input, size)) {
    return GET_INDICATOR;
  } else if (_strings_are_equal(EXIT_COMMAND, input, size)) {
    return EXIT_INDICATOR;
  }
  return NOT_SIMPLE_COMMAND;
}

//If the input is a valid command it executes it, otherwise returns error
static int _execute_command(socket_t *sckt, char *input, size_t size){
  char indicator = _get_command_indicator(input, size);
  if (indicator == NOT_SIMPLE_COMMAND) {
    return _obtain_answer_for_put(sckt, input, strlen(input));
  } else if (indicator == EXIT_INDICATOR) {
    return EXIT_PROGRAM;
  } else {
    return _obtain_answer_simple(sckt, indicator);
  }
}



void _clean_input(char* input, size_t *size){
  if (input[*size-1] == '\n') {
    input[*size-1] = '\0';
    (*size)--;
  }
}


//This function can't be reduced to 15 lines because initialization
//and error/values checking are irreducible
//The ifs can't be united because one ends in returning 1 in main
//and the other one ends returning 0 because it's not an error
//This funcion reads a line from stdin and tries to execute the
//command that in holds, if there is one
static int _process_input(socket_t *sckt){
  char *line = NULL;
  size_t size = 0;
  int program_status = 0;

  size = getline(&line, &size, stdin);
  if (feof(stdin)) {
    free(line);
    return END_OF_FILE;
  }
  if (size == -1) {
    free(line);
    return MEMORY_ERROR;
  }
  _clean_input(line, &size);
  program_status = _execute_command(sckt, line, size);
  free(line);
  return program_status;
}


int client_init(client_t *client, const char *host, const char *service){
  socket_init(&(client->sckt));
  return socket_connect(&(client->sckt), host, service);
}

void client_release(client_t *client){
  socket_release(&(client->sckt));
}

int client_operate(client_t *client){
  int program_status = 0;
  do {
    program_status = _process_input(&(client->sckt));
    _print_error(program_status);
  } while (!_should_kill_program(program_status));
  if (_is_program_terminanting_error(program_status)) {
    return FINISHED_IN_ERROR;
  }
  return SUCCESS;
}
