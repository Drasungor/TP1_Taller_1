#include <stdio.h>
#include <string.h>
#include "remote_sudoku.h"
#include "client.h"
#include "server.h"


#define SUCCESS 0
#define ERROR -1

#define CLIENT_MODE_ARGUMENT "client"
#define SERVER_MODE_ARGUMENT "server"
#define LOCALHOST_MODE_ARGUMENT "localhost"
#define NUMBER_ARGUMENTS_CLIENT 3
#define NUMBER_ARGUMENTS_SERVER 2


static bool _strings_are_equal(const char *str_1,
                               const char *str_2,
                               size_t str_1_size){
  if (str_1_size != strlen(str_2)) {
    return false;
  }
  if (strncmp(str_1, str_2, str_1_size) != 0) {
    return false;
  }
  return true;
}

static int _execute_as_client(const char *host, const char *port){
  client_t client;
  size_t host_len = strlen(host);
  if (_strings_are_equal(host, LOCALHOST_MODE_ARGUMENT, host_len)) {
    host = NULL;
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

static int _execute_as_server(const char *port){
  server_t server;
  if (server_init(&server, port) != SUCCESS) {
    return ERROR;
  }
  if (server_operate(&server) != SUCCESS) {
    return ERROR;
  }
  server_release(&server);
  return SUCCESS;
}

static bool _is_valid_client_command(const char *mode, int number_of_arguments){
  if (number_of_arguments != NUMBER_ARGUMENTS_CLIENT) {
    return false;
  }
  return _strings_are_equal(CLIENT_MODE_ARGUMENT,
                            mode,
                            strlen(CLIENT_MODE_ARGUMENT));
}


static bool _is_valid_server_command(const char *mode, int number_of_arguments){
  if (number_of_arguments != NUMBER_ARGUMENTS_SERVER) {
    return false;
  }
  return _strings_are_equal(SERVER_MODE_ARGUMENT,
                            mode,
                            strlen(SERVER_MODE_ARGUMENT));
}


static void _comunicate_mode_error(const char *mode){
  if (_strings_are_equal(mode, CLIENT_MODE_ARGUMENT, strlen(mode))) {
    fprintf(stderr, "Uso: ./tp client <hostname-servidor> <puerto>");
  } else if (_strings_are_equal(mode, SERVER_MODE_ARGUMENT, strlen(mode))){
    fprintf(stderr, "Uso: ./tp server <puerto>\n");
  }
}


//Returns if the received arguments are potential valid arguments.
//If not it prints an error message
//VER SI HAY QUE CAMBIAR EL NOMBRE, NO ES MUY CLARO
static bool _has_viable_arguments(const char **arguments,
                                 int number_of_arguments){
  if (number_of_arguments < 1) {
    fprintf(stderr, "Modo no sportado, el primer parámatro debe "
                    "ser server o client\n");
    return false;
  }
  if (_strings_are_equal(arguments[0],
                         CLIENT_MODE_ARGUMENT,
                         strlen(arguments[0]))) {
    return true;
  } else if (_strings_are_equal(arguments[0],
                                SERVER_MODE_ARGUMENT,
                                strlen(arguments[0]))){
    return true;
  } else {
    fprintf(stderr, "Modo no sportado, el primer parámatro debe "
                    "ser server o client\n");
    return false;
  }
}


int remote_sudoku_execute(const char **arguments, int number_of_arguments){
  if (!_has_viable_arguments(arguments, number_of_arguments)) {
    return ERROR;
  }
  if (_is_valid_client_command(arguments[0], number_of_arguments)) {
    return _execute_as_client(arguments[1], arguments[2]);
  } else if (_is_valid_server_command(arguments[0], number_of_arguments)) {
    return _execute_as_server(arguments[1]);
  } else {
    _comunicate_mode_error(arguments[0]);
    return ERROR;
  }
  return SUCCESS;
}
