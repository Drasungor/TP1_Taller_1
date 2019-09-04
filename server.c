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


static int send_data(socket_t *sckt, void *message, uint32_t len){
  //uint32_t number_of_chars = strlen(message);

  //ESTÁ MAL LLAMAR A htonl ACÁ? NO ESTÁ A OTRO NIVEL ESTA FUNCIÓN?
  //uint32_t number_to_send = htonl(number_of_chars);

  //CORREGIR, EL NUMERO SE TIENE QUE MANDAR EN LA MISMA TIRA DE BYTES QUE
  //EL STRING QUE HAY QUE IMPRIMIR
  uint32_t number_to_send = htonl(len);
  if (!socket_send(sckt, &number_to_send, sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  if (!socket_send(sckt, message, len)) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}


static char receive_command(socket_t *sckt){
  char command = 0;
  if (!socket_receive(sckt, &command, sizeof(char))) {
    return SOCKET_ERROR;
  }
  return command;
}


//VER SI CONVIENE SACAR LAS CONSTANTES XQ QUEDA UNA DEFINICION MUY LARGA
static void set_char(char matrix[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD], size_t i_offset, size_t j_offset, char c){
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


//CAMBIAR, ES MUY INEFICIENTE
static void initialize_limits(char destination[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD]){
  set_char(destination, 1, 2, '-');
  set_char(destination, 2, 4, '+');
  set_char(destination, 1, 6, '=');
  set_char(destination, 1, 12, 'U');
}


static char int_to_char(int n){
  if (n == 0) {
    return ' ';
  }
  return n+ 48;
}

static void initialize_numbers(int source[9][9], char destination[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD]){
  for (size_t i = 0; i < 9; i++) {
    for (size_t j = 0; j < 9; j++) {
      destination[2 + i * 4][2 + j * 4] = int_to_char(source[i][j]);
    }
  }
}


//CAMBIAR LOS 9 POR CTES
//translates the board returned by sudoku to the one that the server
//has to return
static void process_board(int source[9][9], char destination[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD]){
  initialize_limits(destination);
  initialize_numbers(source, destination);
}

static int get(server_t *server){
  char board[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD];
  int sudoku_board[9][9];
  sudoku_get_board(&(server->sudoku), sudoku_board);
  process_board(sudoku_board, board);
  if (!send_data(&(server->sckt), board, VERTICAL_DIM_PRINTED_BOARD * HORIZONTAL_DIM_PRINTED_BOARD * sizeof(char))) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

static int put(server_t *server){
  char *message = NON_MODIFIABLE_CELL_MESSAGE;
  uint8_t values[PUT_BYTES_RECEIVED-1];

  if (!socket_receive(&(server->sckt), values, (PUT_BYTES_RECEIVED-1) * sizeof(uint8_t))) {
    return SOCKET_ERROR;
  }
  //CAMBIAR EL LLAMADO AL ARRAY EN CADA POSICION POR EL NOMBRE DE UNA VARIABLE ASIGNADA ANTES
  //O PONER DIRECTAMENTE EL INDICE (PERO HACIENDO ESO TAL VEZ NO SE ENTIENDE FACIL AL LEER)
  if (sudoku_set_number(&(server->sudoku), values[PUT_INDEX_NUMBER], values[PUT_INDEX_VERTICAL_POS], values[PUT_INDEX_HORIZONTAL_POS]) != SUCCESS) {
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
  if (!sudoku_verify(&(server->sudoku))) {
    message = SUDOKU_DOESNT_VERIFY;
  }
  if (send_data(&(server->sckt), message, strlen(message)) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

static int reset(server_t *server){
  //AGREGAR CHEQUEOS DE VALORES QUE DEVUELVEN LAS FUNCIONES
  sudoku_reset(&(server->sudoku));
  get(server);



  return SUCCESS;
}


//IMPORTANTE:
//Está bien chequear que me den el comando adecuado?
//xq yo se que el cliente soy yo, asique nunca me voy a mandar
//algo que no sea uno de esos
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

int server_operate(server_t *server){
  //PONER TODO EN UN LOOP DE WHILE IS CONNECTED O ALGO ASI
  int program_state = SUCCESS;
  char command = receive_command(&(server->sckt));
  while ((program_state == SUCCESS) && (command != SOCKET_ERROR)) {
    program_state = process_command(server, command);
    command = receive_command(&(server->sckt));
  }

  //VA A SALIR SOLO CUANDO RECIBA UN ERROR, XQ LO VA A RECIBIR CUANDO CIERRE EL SOCKET
  //DEL CLIENTE

  return SUCCESS;
}
