#ifndef _SERVER_H_
#define  _SERVER_H_


#include "sudoku_handler.h"
#include "socket.h"
#include "client_server_constants.h"

/*
#define SUCCESS 0
#define SOCKET_ERROR -1
#define INVALID_INDICATOR -2
*/

//all attributes are private
typedef struct{
  sudoku_handler_t sudoku_handler;
  socket_t sckt;
}server_t;

//Initializes
int server_init(server_t *server, const char *service);

void server_release(server_t *server);

//Answers to the clients' petitions
//Returns -1 if there is a socket error, otherwise returns 0
//Operates while the client is connected and no error is caused
int server_operate(server_t *server);




#endif
