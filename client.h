#ifndef _CLIENT_H_
#define  _CLIENT_H_

#include "socket.h"



typedef struct{
  socket_t sckt;
}client_t;


int client_init(client_t *client, const char *host, const char *service);

void client_release(client_t *client);

int client_operate(client_t *client);

#endif
