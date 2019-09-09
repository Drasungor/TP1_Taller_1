#define _POSIX_C_SOURCE 200112L
#include <string.h>
#include "server.h"
#include "sudoku_handler.h"
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

static int send_data(socket_t *sckt, void *message, uint32_t len){
  uint32_t number_to_send = htonl(len);
  //HACER CHEQUEO DE SI EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_send(sckt, &number_to_send, sizeof(uint32_t)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  //HACER CHEQUEO DE SI EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_send(sckt, message, len) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}


static char receive_command(socket_t *sckt){
  char command = 0;
  //HACER CHEQUEO DE SI ES QUE EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_receive(sckt, &command, sizeof(char)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return command;
}

static int get(server_t *server){
  size_t elements_printed_board =
  VERTICAL_DIM_PRINTED_BOARD * (HORIZONTAL_DIM_PRINTED_BOARD + 1);
  size_t bytes_to_send = elements_printed_board * sizeof(char);
  char board[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1];

  sudoku_handler_get_board(&(server->sudoku_handler), board);
  //HACER CHEQUEO POR SI EL SOCKET ESTÁ CERRADO
  if (send_data(&(server->sckt), board, bytes_to_send) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

static int put(server_t *server){
  char *message = NON_MODIFIABLE_CELL_MESSAGE;
  uint8_t values[PUT_BYTES_RECEIVED-1];
  size_t bytes_to_send = (PUT_BYTES_RECEIVED-1) * sizeof(uint8_t);
  //HACER CHEQUEO DE SI ES QUE EL SOCKET ESTA CERRADO O SI HUBO UN ERROR
  if (socket_receive(&(server->sckt), values, bytes_to_send) != SUCCESS) {
    return SOCKET_ERROR;
  }
  //CAMBIAR EL LLAMADO AL ARRAY EN CADA POSICION POR EL NOMBRE
  //DE UNA VARIABLE ASIGNADA ANTES O PONER DIRECTAMENTE EL INDICE
  //(PERO HACIENDO ESO TAL VEZ NO SE ENTIENDE FACIL AL LEER)
  int program_status = sudoku_handler_set_number(&(server->sudoku_handler),
//VER SI CONVIENE USAR CONSTANTES
                                                 values[0],
                                                 values[1],
                                                 values[2]);
  if (program_status != SUCCESS) {
    if (send_data(&(server->sckt), message, strlen(message)) != SUCCESS) {
      return SOCKET_ERROR;
    }
  } else {
    //HACER CHEQUEO DE VALOR DE RETORNO DE GET
    get(server);
  }
  return SUCCESS;
}

static int verify(server_t *server){
  char *message = SUDOKU_VERIFIES;
  if (!sudoku_handler_verify(&(server->sudoku_handler))) {
    message = SUDOKU_DOESNT_VERIFY;
  }
  if (send_data(&(server->sckt), message, strlen(message)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

static int reset(server_t *server){
  //AGREGAR CHEQUEOS DE VALORES QUE DEVUELVEN LAS FUNCIONES
  sudoku_handler_reset(&(server->sudoku_handler));
  //AGREGAR CHEQUEO DE GET
  get(server);
  return SUCCESS;
}

//This function can't be reduced to 15 lines because all the indicators have
//to be checked and it makes no sense to separate the indicator checking in
//different functions
static int process_command(server_t *server, char command){
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
  //AGREGAR CHEQUEOS DE LO QUE DEVUELVEN LAS FUNCIONES PARA
  //VER SI HAY QUE DEVOLVER ERROR
  sudoku_handler_init(&(server->sudoku_handler));
  socket_init(&(server->sckt));
  socket_bind_and_listen(&(server->sckt), service);
  socket_accept(&(server->sckt));

  return SUCCESS;
}

void server_release(server_t *server){
  sudoku_handler_release(&(server->sudoku_handler));
  socket_release(&(server->sckt));
}


int server_operate(server_t *server){
  //PONER TODO EN UN LOOP DE WHILE IS CONNECTED O ALGO ASI
  int program_state = SUCCESS;
  char command = receive_command(&(server->sckt));
  while ((program_state == SUCCESS) && (command != SOCKET_ERROR)) {
    program_state = process_command(server, command);
    command = receive_command(&(server->sckt));
  }

  //VA A SALIR SOLO CUANDO RECIBA UN ERROR, XQ LO VA A RECIBIR
  //CUANDO CIERRE EL SOCKET DEL CLIENTE

  return SUCCESS;
}
