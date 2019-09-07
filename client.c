#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "socket.h"

//If there is an error it prints a message that describes it
//VER SI CONVIENE PONER TODOS LOS PRINTS DE ERRORES ACÁ
//LOS DE CADA MODO DE EJECUCION NO ESTÁN ACÁ
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
}

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
//VER SI NO HACE FALTA RECIBIR SIZE DE TODO EL INPUT
//VER SI ESTA MAL QUE LA FUNCION QUE DICE SI ES VALIDO EL COMANDO
//DEVUELVA EN EL ARRAY LAS COORDENADAS
static int is_valid_put(char *input, size_t size, uint8_t data[3]){

  //HACER CHEQUEO DEL CASO EN EL Q NO SE TENGAN LOS SUFICIENTES PEDAZOS
  //DEL COMANDO

  char *first_word = strtok(input, " ");
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
  //VER SI LE MANDO UN STRING "VACIO"
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
  return 0;
}


static void print_message(char* message, size_t size){
  for (size_t i = 0; i < size; i++) {
    printf("%c", message[i]);
  }
}


static int print_answer(socket_t *sckt){
  uint32_t message_size;
  if (!socket_receive(sckt, &message_size, sizeof(uint32_t))) {
    printf("%d\n", ntohl(message_size));
    printf("HAY ERROR DE SOCKET EN RECEIVE\n");
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


//VER SI CONVIENE SACAR EL NOMBRE XQ TAMBIEN LE MANDA UNA
//PETICIÓN
static int obtain_answer(socket_t *sckt, char indicator){
  char indicator_copy = indicator;
  if (!socket_send(sckt, &indicator_copy, sizeof(char))) {
    return SOCKET_ERROR;
  }
  if (print_answer(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }

  //VER SI SE PUEDE MEJOR LA FORMA DE RECIBIR DOS MENSAJES
  //ESTO QUEDA BASTANTE MAL

  return SUCCESS;
}

//TENER ESTA FUNCION A PARTE QUEDA MEDIO RARO, MODIFICAR
static int obtain_answer_for_put(socket_t *sckt, uint8_t coordinates[3]){
  char indicator = PUT_INDICATOR;
  if (!socket_send(sckt, &indicator, sizeof(char))) {
    return SOCKET_ERROR;
  }
  if (!socket_send(sckt, coordinates, 3 * sizeof(uint8_t))) {
    return SOCKET_ERROR;
  }
  if (print_answer(sckt) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

//If the input is a valid command it executes it, otherwise returns error
static int execute_command(socket_t *sckt, char *input, size_t size){
  int program_status = 0;
  //CAMBIAR, NO DEBERIA TENERSTE ESTE ARRAY SOLO PARA UNA DE LAS FUNCIONES
  //QUE SE VAN A LLAMAR
  uint8_t data[3];

  if (strings_are_equal(VERIFY_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, VERIFY_INDICATOR);
  } else if (strings_are_equal(RESET_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, RESET_INDICATOR);
  } else if (strings_are_equal(GET_COMMAND, input, size)) {
    program_status = obtain_answer(sckt, GET_INDICATOR);
  } else if (strings_are_equal(EXIT_COMMAND, input, size)) {
    program_status = EXIT_PROGRAM;
  } else if (is_valid_put(input, size, data) == 0) {
    program_status = obtain_answer_for_put(sckt, data);
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
    print_error(program_state);
  } while (program_state != EXIT_PROGRAM );
  //CAMBIAR PORQUE NO SE ESTA MOSTRANDO CUANDO FALLA EL PROGRAMA
  return SUCCESS;
}
