#define _POSIX_C_SOURCE 200112L
#include <string.h>
#include "server.h"
#include "sudoku.h"
#include "socket.h"

#define SUDOKU_VERIFIES "OK\n"
#define SUDOKU_DOESNT_VERIFY "ERROR\n"
#define BYTES_FOR_PUT 4

char get_command(socket_t *sckt){
  char command = 0;
  if (!socket_receive(sckt, &command, sizeof(char))) {
    return SOCKET_ERROR;
  }
  return command;
}

int get(server_t *server){



  return SUCCESS;
}

int put(server_t *server){



  return SUCCESS;
}

int verify(server_t *server){
  char *message = SUDOKU_VERIFIES;
  if (!sudoku_verify(&(server->sudoku))) {
    message = SUDOKU_DOESNT_VERIFY;
  }
  uint32_t number_of_chars = strlen(message);

  //ESTÁ MAL LLAMAR A htonl ACÁ? NO ESTÁ A OTRO NIVEL ESTA FUNCIÓN?
  uint32_t number_to_send = htonl(number_of_chars);
  if (!socket_send(&(server->sckt), &number_to_send, sizeof(uint32_t))) {
    return SOCKET_ERROR;
  }
  if (!socket_send(&(server->sckt), message, number_of_chars * sizeof(uint32_t))) {
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

  char command = get_command(&(server->sckt));
  //HACER CHEQUEO DE LO QUE DEVUELVE
  process_command(server, command);


  return SUCCESS;
}
