#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"

static bool strings_are_equal(char *command, char *input, size_t size){
  //VER SI CONVIENE PONERLO TODO EN UNA SOLA LINEA
  if (strlen(command) != size) {
    return false;
  }
  if (strncmp(command, input, size) != 0) {
    return false;
  }
  return true;
}


static bool is_valid_position(int position){
  return (position >= 1) && (position <= 9);
}


static bool is_valid_number(int n){
  return (n >= 0) && (n <= 9);
}

//Indicates if the input is a valid format for the command put
static int is_valid_put(char *input, size_t size){

  //HACER CHEQUEO DEL CASO EN EL Q NO SE TENGAN LOS SUFICIENTES PEDAZOS
  //DEL COMANDO

  char *first_word = strtok(input, " ");
  if (!strings_are_equal(VERIFY_COMMAND, first_word, size)) {
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
  //VER SI LE MANDO UN STRING "VACIO"
  char *coordinates = strtok(NULL, "\0");
  if ((coordinates == NULL) || (coordinates[1] != ',')) {
    return INVALID_COMMAND;
  }
  if (!(is_valid_position(atoi(strtok(coordinates, ","))) && is_valid_position(atoi(strtok(NULL, "\0"))))) {
    return INVALID_COORDINATES;
  }
  return 0;
}

/*
int send_indicator(socket_t *sckt, char indicator){
  char indicator_copy = indicator;
  if (!socket_send(sckt, &indicator_copy)) {
  }
  if (!socket_send(sckt, &indicator, sizeof(char))) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}
*/

static void print_message(char* message, size_t size){
  for (size_t i = 0; i < size; i++) {
    printf("%c", message[i]);
  }
}


int print_anser(socket_t *sckt){
  uint32_t message_size;
  if (!socket_receive(sckt, &message_size, sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  message_size = ntohl(message_size);
  char message[message_size];
  if (!socket_receive(sckt, message, message_size)) {
    return SOCKET_ERROR;
  }
  print_message(message, message_size);
  return SUCCESS;
}

bool receives_two_messages(char indicator){
  return (indicator == PUT_INDICATOR)/* || (indicator == RESET_INDICATOR)*/;
}

//VER SI CONVIENE SACAR EL NOMBRE XQ TAMBIEN LE MANDA UNA
//PETICIÓN
static int obtain_answer(socket_t *sckt, char indicator){
  char indicator_copy = indicator;
  if (!socket_send(sckt, &indicator_copy, sizeof(char))) {
    return SOCKET_ERROR;
  }
  /*
  //VER SI CONVIENE HACER UNA FUNCION PRINT_ANSWER O ALGO ASI
  uint32_t message_size;
  if (!socket_receive(sckt, &message_size, sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  message_size = ntohl(message_size);
  char message[message_size];
  if (!socket_receive(sckt, message, message_size)) {
    return SOCKET_ERROR;
  }
  print_message(message, message_size);
  */
  if (print_anser(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }
  //VER SI SE PUEDE MEJOR LA FORMA DE RECIBIR DOS MENSAJES
  //ESTO QUEDA BASTANTE MAL
  /*
  if (receives_two_messages(indicator)) {
    if (print_anser(sckt) != SUCCESS) {
      return SOCKET_ERROR;
    }
  }
  */
  return SUCCESS;
}


//If the input is a valid command it executes it, otherwise returns error
static int execute_command(socket_t *sckt, char *input, size_t size){
  int program_status = 0;

  if (strings_are_equal(VERIFY_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, VERIFY_INDICATOR);
  } else if (strings_are_equal(RESET_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, RESET_INDICATOR);
  } else if (strings_are_equal(GET_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, GET_INDICATOR);
  } else if (strings_are_equal(EXIT_COMMAND, input, size)) {
    program_status = EXIT_PROGRAM;
  } else if (is_valid_put(input, size) == 0) {
    program_status = obtain_answer(sckt, PUT_INDICATOR);
  } else {
    program_status = INVALID_COMMAND;
  }
  return program_status;
}


//VER SI CAMBIO EL NOMBRE POR process_command
static int process_input(socket_t *sckt){
  char *line = NULL;
  size_t size = 0;
  int program_status = 0;

  size = getline(&line, &size, stdin);
  if (size == -1) {
    free(line);
    //NO ES MEMORY ERROR, VER TPS DE ALGO 2
    return MEMORY_ERROR;
  }

  //VER SI ESTE IF ESTÁ A OTRO NIVEL Y DEBERÍA SER PASADO A UNA FUNCIÓN
  if (line[size-1] == '\n') {
    line[size-1] = '\0';
    size--;
  }
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
  int program_state = 0;
  do {
    program_state = process_input(&(client->sckt));
    //IMPLEMENTAR, INDICA AL USUARIO EL ERROR QUE
    //COMETIO AL PONER EL INPUT, SE BASA EN EL VALOR DE
    //PROGRAM STATE
    //comunicate_error(program_state);
  } while (program_state != EXIT_PROGRAM );
  //CAMBIAR PORQUE NO SE ESTA MOSTRANDO CUANDO FALLA EL PROGRAMA
  return SUCCESS;
}
