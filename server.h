#ifndef _SERVER_H_
#define  _SERVER_H_


#include "sudoku.h"
#include "socket.h"

#define GET_INDICATOR 'G'
#define PUT_INDICATOR 'P'
#define VERIFY_INDICATOR 'V'
#define RESET_INDICATOR 'R'

#define SUCCESS 0
#define SOCKET_ERROR -1
#define INVALID_INDICATOR -2

typedef struct{
  sudoku_t sudoku;
  socket_t sckt;
}server_t;




int server_init(server_t *server, const char *service);

void server_release(server_t *server);

int server_operate(server_t *server);




#endif
