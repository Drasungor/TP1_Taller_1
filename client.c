#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "socket.h"

bool strings_are_equal(char *command, char *input, size_t size){
  //VER SI CONVIENE PONERLO TODO EN UNA SOLA LINEA
  if (strlen(command) != size) {
    return false;
  }
  if (strncmp(command, input, size) != 0) {
    return false;
  }
  return true;
}


bool is_valid_position(int position){
  return (position >= 1) && (position <= 9);
}


//Indicates if the input is a valid format for the command put
int is_valid_put(char *input, size_t size){

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
  char *coordinates = strtok(NULL, "\n");
  if ((coordinates == NULL) || (coodrinates[1] != ',')) {
    return INVALID_COMMAND;
  }
  if (!(is_valid_position(int position) && is_valid_position(int position))) {
    return INVALID_COORDINATES;
  }
  return 0;
}

//If the input is a valid command it executes it, otherwise returns error
int validate_input(char *input, size_t size){

  int program_status = 0;

  if (strings_are_equal(VERIFY_COMMAND, input, size)) {
    /* code */
  } else if(strings_are_equal(RESET_COMMAND, input, size)) {

  } else if(strings_are_equal(GET_COMMAND, input, size)) {

  } else if(strings_are_equal(EXIT_COMMAND, input, size)) {

  } else if(is_valid_put(input, size) == 0) {

  } else {
    program_status = INVALID_COMMAND;
  }
  return program_status;
}


//VER SI CAMBIO EL NOMBRE POR process_command
int process_input(){
  char *line = NULL;
  size_t size = 0;

  size = getline(&line, &size, stdin);
  if (size == -1) {
    free(line);
    //NO ES MEMORY ERROR, VER TPS DE ALGO 2
    return MEMORY_ERROR;
  }


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

void client_operate(client_t *client){



}
