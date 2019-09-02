#define _POSIX_C_SOURCE 200112L
#include <string.h>
#include "server.h"
#include "sudoku.h"
#include "socket.h"

#define SUDOKU_VERIFIES "OK\n"
#define SUDOKU_DOESNT_VERIFY "ERROR\n"
#define PUT_BYTES_RECEIVED 4
#define PUT_INDEX_NUMBER 0
#define PUT_INDEX_VERTICAL_POS 1
#define PUT_INDEX_HORIZONTAL_POS 2
#define NON_MODIFIABLE_CELL_MESSAGE "La celda indicada no es modificable\n"
#define HORIZONTAL_DIM_PRINTED_BOARD 37
#define VERTICAL_DIM_PRINTED_BOARD 19


int send_message(socket_t *sckt, char *message){
  uint32_t number_of_chars = strlen(message);

  //ESTÁ MAL LLAMAR A htonl ACÁ? NO ESTÁ A OTRO NIVEL ESTA FUNCIÓN?
  uint32_t number_to_send = htonl(number_of_chars);
  if (!socket_send(sckt, &number_to_send, sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  if (!socket_send(sckt, message, number_of_chars * sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}


char receive_command(socket_t *sckt){
  char command = 0;
  if (!socket_receive(sckt, &command, sizeof(char))) {
    return SOCKET_ERROR;
  }
  return command;
}


//VER SI CONVIENE SACAR LAS CONSTANTES XQ QUEDA UNA DEFINICION MUY LARGA
void set_char(char matrix[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD], size_t i_offset, size_t j_offset, char c){
  //VER SI CONVIENE CAMBIAR COMO FUNCIONA LA FUNCION XQ CÓMO QUEDA ESCRITO
  //DEPENDE DEL ORDEN EN EL QUE SE SETEEN LOS CARACTERES, ADEMAS
  //HACE INICIALIZACIONES INNECESARIAS XQ DESPUES SE VAN A SOBRESCRIBIR
  //CON OTROS CARACTERES
  for (size_t i = 0; i < VERTICAL_DIM_PRINTED_BOARD; i += i_offset) {
    for (size_t j = 0; j < HORIZONTAL_DIM_PRINTED_BOARD; j += j_offset) {
      matrix[i][j] = c;
    }
  }
}


void initialize_limits(char destination[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD]){
  set_char(destination, 1, 2, '-');
  set_char(destination, 2, 4, '+');
  set_char(destination, 1, 6, '=');
}

//CAMBIAR LOS 9 POR CTES
//translates the board returned by sudoku to the one that the server
//has to return
void process_board(int source[9][9], char destination[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD]){

}

int get(server_t *server){



  return SUCCESS;
}

int put(server_t *server){
  char *message = NON_MODIFIABLE_CELL_MESSAGE;
  uint8_t values[PUT_BYTES_RECEIVED-1];

  if (!socket_receive(&(server->sckt), values, (PUT_BYTES_RECEIVED-1) * sizeof(uint8_t))) {
    return SOCKET_ERROR;
  }
  //CAMBIAR EL LLAMADO AL ARRAY EN CADA POSICION POR EL NOMBRE DE UNA VARIABLE ASIGNADA ANTES
  //O PONER DIRECTAMENTE EL INDICE (PERO HACIENDO ESO TAL VEZ NO SE ENTIENDE FACIL AL LEER)
  if (sudoku_set_number(&(server->sudoku), values[PUT_INDEX_NUMBER], values[PUT_INDEX_VERTICAL_POS], values[PUT_INDEX_HORIZONTAL_POS]) != SUCCESS) {
    if (send_message(&(server->sckt), message) != SUCCESS) {
      return SOCKET_ERROR;
    }
  } else {
    //HACER CHEQUEO DE VALOR DE RETORNO DE GET
    get(server);
  }

  return SUCCESS;
}

int verify(server_t *server){
  char *message = SUDOKU_VERIFIES;
  if (!sudoku_verify(&(server->sudoku))) {
    message = SUDOKU_DOESNT_VERIFY;
  }
  if (send_message(&(server->sckt), message) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

int reset(server_t *server){
  //AGREGAR CHEQUEOS DE VALORES QUE DEVUELVEN LAS FUNCIONES
  sudoku_reset(&(server->sudoku));
  get(server);



  return SUCCESS;
}


//IMPORTANTE:
//Está bien chequear que me den el comando adecuado?
//xq yo se que el cliente soy yo, asique nunca me voy a mandar
//algo que no sea uno de esos
int process_command(server_t *server, char command){
  int program_state = SUCCESS;
  switch (command) {
    case GET_INDICATOR:
      program_state = get(server);
      break;
    case PUT_INDICATOR:
      program_state = put(server);
      break;
    case VERIFY_INDICATOR:
      program_state = verify(server);
      break;
    case RESET_INDICATOR:
      program_state = reset(server);
      break;
    default:
      program_state = INVALID_INDICATOR;
      break;
  }
  return program_state;
}




int server_init(server_t *server, const char *service){

  //AGREGAR CHEQUEOS DE LO QUE DEVUELVEN LAS FUNCIONES PARA VER SI HAY QUE DEVOLVER ERROR
  sudoku_init_with_file(&(server->sudoku));
  socket_init(&(server->sckt));
  socket_bind_and_listen(&(server->sckt), service);
  socket_accept(&(server->sckt));

  return SUCCESS ;
}

void server_release(server_t *server){
  sudoku_release(&(server->sudoku));
  socket_release(&(server->sckt));
}

int operate(server_t *server){
  //PONER TODO EN UN LOOP DE WHILE IS CONNECTED O ALGO ASI

  char command = receive_command(&(server->sckt));
  //HACER CHEQUEO DE LO QUE DEVUELVE
  process_command(server, command);


  return SUCCESS;
}
