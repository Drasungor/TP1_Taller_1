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

//#define INVALID_MODE_ERROR_MESSAGE "Modo no sportado,
//el primer parámatro debe ser server o client\n"
#define INVALID_CLIENT_ERROR_MESSAGE "Uso: ./tp client <host> <puerto>\n"
#define INVALID_SERVER_ERROR_MESSAGE "Uso: ./tp server <puerto>\n"

static bool strings_are_equal(const char *str_1,
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

static int execute_as_client(const char *host, const char *port){
  client_t client;
  //VER SI CONVIENE HACER UNA FUNCION A PARTE QUE HAGA EL CHEQUEO DE
  //SI SON EL MISMO STRING
  size_t host_len = strlen(host);
  if (strings_are_equal(host, LOCALHOST_MODE_ARGUMENT, host_len)) {
    host = NULL;
  }
  //ESPECIFICAR EL TIPO DE ERROR QUE SE TIENE QUE DEVOLVER
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
  /*
  size_t client_mode_lenght = strlen(CLIENT_MODE_ARGUMENT);
  if (strlen(mode) != client_mode_lenght) {
    return false;
  }
  if (strncmp(mode, CLIENT_MODE_ARGUMENT, client_mode_lenght) != 0) {
    return false;
  }
  */
  return strings_are_equal(CLIENT_MODE_ARGUMENT,
                           mode,
                           strlen(CLIENT_MODE_ARGUMENT));
}


static bool is_valid_server_command(const char *mode, int number_of_arguments){
  if (number_of_arguments != NUMBER_ARGUMENTS_SERVER) {
    return false;
  }
  return strings_are_equal(SERVER_MODE_ARGUMENT,
                           mode,
                           strlen(SERVER_MODE_ARGUMENT));
}


//VER SI ESTA FUNCION COMENTADA ES MEJOR QUE LA OTRA strings_are_equal
/*
static bool strings_are_equal(const char *string_1, const char *string_2){
  size_t string_1_len = strlen(string_1);
  if (string_1_len == strlen(string_2)) {
    return strncmp(string_1, string_2, string_1_len) == 0;
  }
  return false;
}
*/

static void comunicate_mode_error(const char *mode){
  if (strings_are_equal(mode, CLIENT_MODE_ARGUMENT, strlen(mode))) {
    fprintf(stderr, INVALID_CLIENT_ERROR_MESSAGE);
  } else if (strings_are_equal(mode, SERVER_MODE_ARGUMENT, strlen(mode))){
    fprintf(stderr, INVALID_SERVER_ERROR_MESSAGE);
  }
}


//Returns if the received arguments are potential valid arguments.
//If not it prints an error message
//VER SI HAY QUE CAMBIAR EL NOMBRE, NO ES MUY CLARO
static bool has_viable_arguments(const char **arguments,
                                 int number_of_arguments){
  if (number_of_arguments < 1) {
    fprintf(stderr, "Modo no sportado, el primer parámatro debe "
                    "ser server o client\n");
    return false;
  }
  if (strings_are_equal(arguments[0],
                        CLIENT_MODE_ARGUMENT,
                        strlen(arguments[0]))) {
    return true;
  } else if (strings_are_equal(arguments[0],
                               SERVER_MODE_ARGUMENT,
                               strlen(arguments[0]))){
    return true;
  } else {
    fprintf(stderr, "Modo no sportado, el primer parámatro debe "
                    "ser server o client\n");
    return false;
  }
}


int remote_sudoku_start(const char **arguments, int number_of_arguments){
  if (!has_viable_arguments(arguments, number_of_arguments)) {
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
