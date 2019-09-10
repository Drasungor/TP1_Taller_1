#include <string.h>
#include "server.h"
#include "sudoku_handler.h"
#include "socket.h"

#define SUDOKU_VERIFIES "OK\n"
#define SUDOKU_DOESNT_VERIFY "ERROR\n"
#define PUT_BYTES_RECEIVED 4
#define NUMBER_INDEX 0
#define VER_POS_INDEX 1
#define HOR_POS_INDEX 2
#define NON_MODIFIABLE_CELL_MESSAGE "La celda indicada no es modificable\n"
#define HORIZONTAL_DIM_PRINTED_BOARD 37
#define VERTICAL_DIM_PRINTED_BOARD 19


#define SUCCESS 0
#define ERROR -1
#define CLOSED_SOCKET -2
#define FINISHED_IN_ERROR 1


static int _send_data(socket_t *sckt, void *message, uint32_t len){
  uint32_t number_to_send = htonl(len);
  int program_status = socket_send(sckt, &number_to_send, sizeof(uint32_t));
  if (program_status != SUCCESS) {
    return program_status;
  }
  return socket_send(sckt, message, len);
}


static char _receive_command(socket_t *sckt){
  char command = 0;
  int program_status = socket_receive(sckt, &command, sizeof(char));
  if (program_status != SUCCESS) {
    return program_status;
  }
  return command;
}

static int _get(server_t *server){
  size_t elements_printed_board =
  VERTICAL_DIM_PRINTED_BOARD * (HORIZONTAL_DIM_PRINTED_BOARD + 1);
  size_t bytes_to_send = elements_printed_board * sizeof(char);
  char board[VERTICAL_DIM_PRINTED_BOARD][HORIZONTAL_DIM_PRINTED_BOARD + 1];

  sudoku_handler_get_board(&(server->sudoku_handler), board);
  return _send_data(&(server->sckt), board, bytes_to_send);
}

static int _put(server_t *server){
  char *message = NON_MODIFIABLE_CELL_MESSAGE;
  uint8_t values[PUT_BYTES_RECEIVED-1];
  size_t bytes_to_send = (PUT_BYTES_RECEIVED-1) * sizeof(uint8_t);
  int program_status = socket_receive(&(server->sckt), values, bytes_to_send);
  if (program_status != SUCCESS) {
    return program_status;
  }
  program_status = sudoku_handler_set_number(&(server->sudoku_handler),
                                             values[NUMBER_INDEX],
                                             values[VER_POS_INDEX],
                                             values[HOR_POS_INDEX]);
  if (program_status != SUCCESS) {
    program_status = _send_data(&(server->sckt), message, strlen(message));
    if (program_status != SUCCESS) {
      return program_status;
    }
  }
  return _get(server);
}

static int _verify(server_t *server){
  char *message = SUDOKU_VERIFIES;
  if (!sudoku_handler_verify(&(server->sudoku_handler))) {
    message = SUDOKU_DOESNT_VERIFY;
  }
  return _send_data(&(server->sckt), message, strlen(message));
}

static int _reset(server_t *server){
  sudoku_handler_reset(&(server->sudoku_handler));
  return _get(server);
}

//This function can't be reduced to 15 lines because all the indicators have
//to be checked and it makes no sense to separate the indicator checking in
//different functions
static int _process_command(server_t *server, char command){
  int program_state = SUCCESS;
  switch (command) {
    case GET_INDICATOR:
      program_state = _get(server);
      break;
    case PUT_INDICATOR:
      program_state = _put(server);
      break;
    case VERIFY_INDICATOR:
      program_state = _verify(server);
      break;
    case RESET_INDICATOR:
      program_state = _reset(server);
      break;
  }
  return program_state;
}


static bool _program_should_run(int program_status, char command){
  return program_status == SUCCESS &&
         command != ERROR &&
         command != CLOSED_SOCKET;
}

int server_init(server_t *server, const char *service){
  int program_state = sudoku_handler_init(&(server->sudoku_handler));
  if (program_state != SUCCESS) {
    return ERROR;
  }
  socket_init(&(server->sckt));
  program_state = socket_bind_and_listen(&(server->sckt), service);
  if (program_state != SUCCESS) {
    return ERROR;
  }
  return socket_accept(&(server->sckt));
}

void server_release(server_t *server){
  sudoku_handler_release(&(server->sudoku_handler));
  socket_release(&(server->sckt));
}


int server_operate(server_t *server){
  int program_status = SUCCESS;
  char command = _receive_command(&(server->sckt));
  while (_program_should_run(program_status, command)) {
    program_status = _process_command(server, command);
    command = _receive_command(&(server->sckt));
  }
  if ((command == ERROR) || (program_status == ERROR)) {
    return FINISHED_IN_ERROR;
  }
  return SUCCESS;
}
