#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "socket.h"


void iterate_addrinfo_list(struct addrinfo *element, bool (*visit)(struct addrinfo*, void *extra), void *extra){
  while ((element != NULL) && (visit(element, extra))) {
    element = element->ai_next;
  }
}

//Tries to connect a socket to the host.
//Returns true if it fails, false otherwise
bool process_info(struct addrinfo* info, void *extra){

}

void socket_init(socket_t *socket){
  socket->file_descriptor = 0;
  memset(&(socket->hints), 0, sizeof(struct addrinfo));
  socket->hints.ai_family = AF_INET;
  socket->hints.ai_socktype = SOCK_STREAM;
}

void socket_release(socket_t *socket){
}

int socket_bind_and_listen(socket_t *socket, const char *service){
  return 0;
}

int socket_connect(socket_t *socket, const char *host, const char *service){
  int info_result = 0;
  socket->hints.ai_flags = 0;
  struct addrinfo *result;

  info_result = getaddrinfo(host, service, &(socket->hints), &result);

  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
    return info_result;
  }

  return 0;
}
