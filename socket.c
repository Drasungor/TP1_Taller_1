#define _POSIX_C_SOURCE 200112L
#include "socket.h"








void socket_init(socket_t *socket){
}

void socket_release(socket_t *socket){
}

int socket_bind_and_listen(socket_t *socket, const char *service){
  return 0;
}

int socket_connect(socket_t *socket, const char *host, const char *service){
  return 0;
}
