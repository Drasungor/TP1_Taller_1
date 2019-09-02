#include "client.h"
#include "socket.h"











int client_init(client_t *client, const char *host, const char *service){
  socket_init(&(client->sckt));
  if (socket_connect(&(client->sckt), host, service) != SUCCESS) {
    return SOCKET_ERROR;
  }
  return SUCCESS;
}

void client_release(client_t *client){
  socket_release(&(client->sckt));
}

void client_operate(client_t *client){



}
