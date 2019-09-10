#ifndef _CLIENT_H_
#define  _CLIENT_H_

#include "socket.h"


//all attributes are private
typedef struct{
  socket_t sckt;
}client_t;


//Initializes the client's elements
//Returns 0 if it's successful, -1 if there is a socket error
int client_init(client_t *client, const char *host, const char *service);


void client_release(client_t *client);


//Executes all the commands specified in stdin, killing the process
//when EOF or the exit are found. If there is a memory or socket error
//the process is also killed and returns 1. otherwise returns 0
int client_operate(client_t *client);

#endif
