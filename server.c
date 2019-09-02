#include "server.h"
#include "sudoku.h"
#include "socket.h"




char get_command(socket_t *sckt){
  unsigned char command = 0;
  if (!soket_receive(sckt, &command, sizeof(unsigned char))) {
    return SOCKET_ERROR;
  }
  return command;
}

int process_command(socket_t *sckt, char command){
  
}

int server_init(server_t *server, const char *service){

  //AGREGAR CHEQUEOS DE LO QUE DEVUELVEN LAS FUNCIONES PARA VER SI HAY QUE DEVOLVER ERROR
  sudoku_init_with_file(&(server->sudoku));
  socket_init(&(server->sckt));
  socket_bind_and_listen(&(server->sckt), service);
  socket_accept(&(server->sckt));
}

void server_release(server_t *server){
  sudoku_release(&(server->sudoku));
  socket_release(&(server->sckt));
}

int operate(server_t *server){
  //PONER TODO EN UN LOOP DE WHILE IS CONNECTED O ALGO ASI

  char command = get_command(&(server->sckt));
  //HACER CHEQUEO DE LO QUE DEVUELVE
  process_command(&(server->sckt), command);

}
