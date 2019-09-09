#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"


#define GET_INDICATOR 'G'
#define PUT_INDICATOR 'P'
#define VERIFY_INDICATOR 'V'
#define RESET_INDICATOR 'R'
#define EXIT_INDICATOR 'E'
#define GET_COMMAND "get"
#define PUT_COMMAND "put"
#define VERIFY_COMMAND "verify"
#define RESET_COMMAND "reset"
#define EXIT_COMMAND "exit"

#define SUCCESS 0
#define SOCKET_ERROR -1
#define MEMORY_ERROR -2
#define INVALID_COMMAND -3
#define INVALID_NUMBER -4
#define INVALID_COORDINATES -5
#define EXIT_PROGRAM -6
#define END_OF_FILE -7
#define CLOSED_SOCKET -8
#define NOT_SIMPLE_COMMAND -9

//If there is an error it prints a message that describes it
static void print_error(int program_status){
  if (program_status == SOCKET_ERROR) {
    fprintf(stderr, "Error de conexión\n");
  }
  if (program_status == INVALID_NUMBER) {
    fprintf(stderr, "Error en el valor ingresado. Rango soportado: [1,9]\n");
  }
  if (program_status == INVALID_COORDINATES) {
    fprintf(stderr, "Error en los índices. Rango soportado: [1,9]\n");
  }
  if (program_status == MEMORY_ERROR) {
    fprintf(stderr, "Error en la asignación de memoria dinámica\n");
  }
}

static bool strings_are_equal(char *command, char *input, size_t size){
  if (strlen(command) != size) {
    return false;
  }
  if (strncmp(command, input, size) != 0) {
    return false;
  }
  return true;
}


static bool is_program_terminanting_error(int program_status){
  if (program_status == SOCKET_ERROR) {
    return true;
  }
  if (program_status == MEMORY_ERROR) {
    return true;
  }
  return false;
}

static bool is_program_terminanting_value(int program_status){
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

static bool should_kill_program(int program_status){
  return is_program_terminanting_value(program_status) || is_program_terminanting_error(program_status);
}


static bool is_valid_position(int position){
  return (position >= 1) && (position <= 9);
}


static bool is_valid_number(int n){
  return (n >= 1) && (n <= 9);
}

//Indicates if the input is a valid format for the command put
static int put_command_validation(char *input, size_t size, uint8_t data[3]){
  char *first_word = strtok(input, " ");
  if (first_word == NULL) {
    return INVALID_COMMAND;
  }
  if (!strings_are_equal(PUT_COMMAND, first_word, strlen(first_word))) {
    return INVALID_COMMAND;
  }
  char *number = strtok(NULL, " ");
  if (number == NULL) {
    return INVALID_COMMAND;
  }
  if (!is_valid_number(atoi(number))) {
    return INVALID_NUMBER;
  }
  char *nexus = strtok(NULL, " ");
  if ((nexus == NULL) || (!strings_are_equal("in", nexus, strlen(nexus)))) {
    return INVALID_COMMAND;
  }
  char *coordinates = strtok(NULL, "\0");
  if ((coordinates == NULL) || (coordinates[1] != ',')) {
    return INVALID_COMMAND;
  }
  uint8_t i = atoi(strtok(coordinates, ","));
  uint8_t j = atoi(strtok(NULL, "\0"));
  if (!(is_valid_position(i) && is_valid_position(j))) {
    return INVALID_COORDINATES;
  }
  //VER SI ESTO ESTA MAL, BUSCAR OTRA FORMA DE HACERLO
  data[0] = atoi(number);
  data[1] = i;
  data[2] = j;
  return SUCCESS;
}

static void print_message(char* message, size_t size){
  message[size] = '\0';
  printf("%s", message);
}

static int print_answer(socket_t *sckt){
  uint32_t message_size;
  int program_status = socket_receive(sckt, &message_size, sizeof(uint32_t));
  if (program_status != SUCCESS) {
    return program_status;
  }
  message_size = ntohl(message_size);
  char message[message_size+1];
  program_status = socket_receive(sckt, message, message_size);
  if (program_status != SUCCESS) {
    return program_status;
  }
  print_message(message, message_size);
  return SUCCESS;
}


//VER SI CONVIENE SACAR EL NOMBRE XQ TAMBIEN LE MANDA UNA
//PETICIÓN
static int obtain_answer_simple(socket_t *sckt, char indicator){
  char indicator_copy = indicator;
  int program_status = socket_send(sckt, &indicator_copy, sizeof(char));
  if (program_status != SUCCESS) {
    //VER SI HAY QUE MODIFICAR ESTO UN POCO PARA QUE TAMBIEN RECIBA CLOSED SOCKET
    return SOCKET_ERROR;
  }
  if (print_answer(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

/*
static int obtain_answer_for_put(socket_t *sckt, uint8_t coordinates[3]){
  char indicator = PUT_INDICATOR;
  //HACER CHEQUEO DE SI EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_send(sckt, &indicator, sizeof(char)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  //HACER CHEQUEO DE SI EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_send(sckt, coordinates, 3 * sizeof(uint8_t)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  if (print_answer(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}
*/

//Receives
static int obtain_answer_for_put(socket_t *sckt, uint8_t coordinates[3]){

  //VER SI ESTE BLOQUE DE CODIGO DEBERIA IR EN OTRO LADO
  uint8_t data[3];
  int put_validation = put_command_validation(input, size, data);
  if (put_validation != SUCCESS) {
    return put_validation;
  }


  if (socket_send(sckt, &indicator, sizeof(char)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  //HACER CHEQUEO DE SI EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_send(sckt, coordinates, 3 * sizeof(uint8_t)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  if (print_answer(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}


//Returns the indicator associated to the command given
//If the command does not exist ir returns INVALID_COMMAND
static char get_command_indicator(char *input, size_t size){
  if (strings_are_equal(VERIFY_COMMAND, input, size)) {
    return VERIFY_INDICATOR;
  } else if (strings_are_equal(RESET_COMMAND, input, size)) {
    return RESET_INDICATOR;
  } else if (strings_are_equal(GET_COMMAND, input, size)) {
    return GET_INDICATOR;
  } else if (strings_are_equal(EXIT_COMMAND, input, size)) {
    return EXIT_INDICATOR;
  }/* else if (strings_are_equal(PUT_COMMAND, input, size)){
    return PUT_INDICATOR;
  }
  */
  return NOT_SIMPLE_COMMAND;
}

//If the input is a valid command it executes it, otherwise returns error
static int execute_command(socket_t *sckt, char *input, size_t size){
  //int program_status = 0;
  //uint8_t data[3];
  //int put_validation = put_command_validation(input, size, data);
  //CAMBIAR, NO DEBERIA TENERSTE ESTE ARRAY SOLO PARA UNA DE LAS FUNCIONES
  //QUE SE VAN A LLAMAR
  /*
  if (strings_are_equal(VERIFY_COMMAND, input, size)) {
    program_status = obtain_answer_simple(sckt, VERIFY_INDICATOR);
  } else if (strings_are_equal(RESET_COMMAND, input, size)) {
    program_status = obtain_answer_simple(sckt, RESET_INDICATOR);
  } else if (strings_are_equal(GET_COMMAND, input, size)) {
    program_status = obtain_answer_simple(sckt, GET_INDICATOR);
  } else if (strings_are_equal(EXIT_COMMAND, input, size)) {
    program_status = EXIT_PROGRAM;
  } else if (put_validation != INVALID_COMMAND) {
    if (put_validation == SUCCESS) {
      program_status = obtain_answer_for_put(sckt, data);
    } else {
      program_status = put_validation;
    }
  } else {
    program_status = INVALID_COMMAND;
  }
  */
  char indicator = get_command_indicator(input, size);
  if (indicator == NOT_SIMPLE_COMMAND) {
    //return indicator;
    return obtain_answer_for_put(sckt);
  } else if (indicator == EXIT_INDICATOR) {
    return EXIT_PROGRAM;
  } /*else if (indicator == PUT_INDICATOR) {
    return obtain_answer_for_put(sckt);
  } */else {
    return obtain_answer_simple(sckt, indicator);
  }
  //return program_status;
}



void clean_input(char* input, size_t *size){
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
static int process_input(socket_t *sckt){
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
  clean_input(line, &size);
  program_status = execute_command(sckt, line, size);
  free(line);
  return program_status;
}


int client_init(client_t *client, const char *host, const char *service){
  socket_init(&(client->sckt));
  if (socket_connect(&(client->sckt), host, service) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

void client_release(client_t *client){
  socket_release(&(client->sckt));
}


int client_operate(client_t *client){
  int program_status = 0;
  do {
    program_status = process_input(&(client->sckt));
    print_error(program_status);
  } while (!should_kill_program(program_status));
  if (is_program_terminanting_error(program_status)) {
    return 1;
  }
  return 0;
}
