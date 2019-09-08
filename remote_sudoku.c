#include <stdio.h>
#include <string.h>
#include "remote_sudoku.h"
#include "client.h"
#include "server.h"

#define CLIENT_MODE_ARGUMENT "client"
#define SERVER_MODE_ARGUMENT "server"
#define LOCALHOST_MODE_ARGUMENT "localhost"
//VER SI LO DEJO COMO UN SOLO DEFINE XQ SON EL MISMO VALOR,
//PERO SI AGREGO OTRO COMANDO MAS DESPUES QUE TIENE UNA CANTIDAD
//DISTINTA DE PARAMETROS SE ROMPE TODO
#define NUMBER_ARGUMENTS_CLIENT 3
#define NUMBER_ARGUMENTS_SERVER 2

static int execute_as_client(const char *host, const char *port){
  //int program_state = 0;
  client_t client;
  //VER QUE CONSTANTES VOY A USAR PARA DEVOLVER A MAIN
  //VER SI SE PUEDEN UNIFICAR ESTOS CHEQUEOS DE ERRORES
  //CON ELSE IF

  //VER SI CONVIENE HACER UNA FUNCION A PARTE QUE HAGA EL CHEQUEO DE
  //SI SON EL MISMO STRING
  size_t host_len = strlen(host);
  if (host_len == strlen(LOCALHOST_MODE_ARGUMENT)) {
    if (strncmp(host, LOCALHOST_MODE_ARGUMENT, host_len) == 0) {
      host = NULL;
    }
  }
  if (client_init(&client, host, port) != SUCCESS) {
    return ERROR;
  }
  if (client_operate(&client) != SUCCESS) {
    return ERROR;
  }
  client_release(&client);
  return SUCCESS;
}

static int execute_as_server(const char *port){
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

static bool is_valid_client_command(const char *mode, int number_of_arguments){
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


static bool is_valid_server_command(const char *mode, int number_of_arguments){
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

//CAMBIAR LA LOGICA DE ESTA FUNCION PORQUE ES MALISIMA
static void comunicate_mode_error(const char *mode){
  size_t mode_len = strlen(mode);
  if (mode_len == strlen(CLIENT_MODE_ARGUMENT)) {
    if (strncmp(mode, CLIENT_MODE_ARGUMENT, mode_len) == 0) {
      fprintf(stderr, "Uso: ./tp client <host> <puerto>\n");
    }
  }
  if (mode_len == strlen(SERVER_MODE_ARGUMENT)) {
    if (strncmp(mode, SERVER_MODE_ARGUMENT, mode_len) == 0) {
      fprintf(stderr, "Uso: ./tp server <puerto>\n");
    }
  } else {
    fprintf(stderr, "Modo no sportado, el primer parámatro debe ser sever o client\n");
  }
}


bool has_enough_arguments(int number_of_arguments){
  return number_of_arguments >= 1;
}

int remote_sudoku_start(const char **arguments, int number_of_arguments){
  //CAMBIAR ESTO PORQUE NO TIRA EL ERROR IMPRESO
  //ADEMAS NO ESTA AL "MISMO NIVEL" QUE EL RESTO DE LAS COSAS
  //DE LA FUNCION
  if (!has_enough_arguments(number_of_arguments)) {
    return ERROR;
  }
  if (is_valid_client_command(arguments[0], number_of_arguments)) {
    return execute_as_client(arguments[1], arguments[2]);
  } else if (is_valid_server_command(arguments[0], number_of_arguments)) {
    return execute_as_server(arguments[1]);
  } else {
    comunicate_mode_error(arguments[0]);
    return ERROR;
  }
  return SUCCESS;
}
