#include "remote_sudoku.h"
#include "client.h"
#include "server.h"



int execute_as_client(){
  int program_state = 0;
  client_t client;
  //VER QUE CONSTANTES VOY A USAR PARA DEVOLVER A MAIN
  //VER SI SE PUEDEN UNIFICAR ESTOS CHEQUEOS DE ERRORES
  //CON ELSE IF
  if (client_init(&client) != SUCCESS) {
    return ERROR;
  }
  if (client_operate(&client) != SUCCESS) {
    return ERROR;
  }
  client_release(&client);
  return SUCCESS;
}

int execute_as_server(){
  int program_state = 0;
  server_t server;
  //VER QUE CONSTANTES VOY A USAR PARA DEVOLVER A MAIN
  //VER SI SE PUEDEN UNIFICAR ESTOS CHEQUEOS DE ERRORES
  //CON ELSE IF
  if (server_init(&server) != SUCCESS) {
    return ERROR;
  }
  if (server_operate(&server) != SUCCESS) {
    return ERROR;
  }
  server_release(&server);
  return SUCCESS;
}


int remote_sudoku_start(char **arguments, int number_of_arguments){
  if (is_valid_client_command()) {
    return execute_as_client();
  } else if (is_valid_server_command()) {
    return execute_as_server();
  } else {
    return ERROR;
  }
}
