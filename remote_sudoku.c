#include <string.h>
#include "remote_sudoku.h"
#include "client.h"
#include "server.h"

#define CLIENT_MODE_ARGUMENT "client"
#define SERVER_MODE_ARGUMENT "server"

//VER SI LO DEJO COMO UN SOLO DEFINE XQ SON EL MISMO VALOR,
//PERO SI AGREGO OTRO COMANDO MAS DESPUES QUE TIENE UNA CANTIDAD
//DISTINTA DE PARAMETROS SE ROMPE TODO
#define NUMBER_ARGUMENTS_CLIENT 3
#define NUMBER_ARGUMENTS_SERVER 2

int execute_as_client(const char *host, const char *port){
  //int program_state = 0;
  client_t client;
  //VER QUE CONSTANTES VOY A USAR PARA DEVOLVER A MAIN
  //VER SI SE PUEDEN UNIFICAR ESTOS CHEQUEOS DE ERRORES
  //CON ELSE IF
  if (client_init(&client, host, port) != SUCCESS) {
    return ERROR;
  }
  client_operate(&client);
  /*
  if (client_operate(&client) != SUCCESS) {
    return ERROR;
  }
  */
  client_release(&client);
  return SUCCESS;
}

int execute_as_server(const char *port){
  //int program_state = 0;
  server_t server;
  //VER QUE CONSTANTES VOY A USAR PARA DEVOLVER A MAIN
  //VER SI SE PUEDEN UNIFICAR ESTOS CHEQUEOS DE ERRORES
  //CON ELSE IF
  if (server_init(&server, port) != SUCCESS) {
    return ERROR;
  }
  if (server_operate(&server) != SUCCESS) {
    return ERROR;
  }
  server_release(&server);
  return SUCCESS;
}

bool is_valid_client_command(const char *mode, int number_of_arguments){
  if (number_of_arguments != NUMBER_ARGUMENTS_CLIENT) {
    return false;
  }
  size_t client_mode_lenght = strlen(CLIENT_MODE_ARGUMENT);
  if (strlen(mode) != client_mode_lenght) {
    return false;
  }
  if (strncmp(mode, CLIENT_MODE_ARGUMENT, client_mode_lenght) != 0) {
    return false;
  }
  return true;
}


//VER SI CONVIENE HACERLA UNA MISMA FUNCION (CON LA DE CLIENT) CON MAS PARAMETROS
bool is_valid_server_command(const char *mode, int number_of_arguments){
  if (number_of_arguments != NUMBER_ARGUMENTS_SERVER) {
    return false;
  }
  size_t server_mode_lenght = strlen(SERVER_MODE_ARGUMENT);
  if (strlen(mode) != server_mode_lenght) {
    return false;
  }
  if (strncmp(mode, SERVER_MODE_ARGUMENT, server_mode_lenght) != 0) {
    return false;
  }
  return true;
}

int remote_sudoku_start(const char **arguments, int number_of_arguments){
  if (is_valid_client_command(arguments[0], number_of_arguments)) {
    return execute_as_client(arguments[1], arguments[2]);
  } else if (is_valid_server_command(arguments[0], number_of_arguments)) {
    return execute_as_server(arguments[1]);
  } else {
    return ERROR;
  }
}
