#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "socket.h"


//Iterates the list while the visit function
//returns true
void iterate_addrinfo_list(struct addrinfo *element, bool (*visit)(struct addrinfo*, void *extra), void *extra){
  while ((element != NULL) && (visit(element, extra))) {
    element = element->ai_next;
  }
}

//Tries to connect a socket to the host.
//Returns true if it fails, false otherwise
bool process_info_to_connect(struct addrinfo* info, void *extra){
  int socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  int connect_value = 0;
  bool *is_connected = (bool*)extra;

  if (socket == -1) {
    return true;
  }
  connect_value = connect(socket_fd, info->ai_addr, info->ai_addrlen);
  if (connect_value == -1) {
    return true;
  }
  *is_connected = false;
  return false;
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
  bool is_connected = false;
  socket->hints.ai_flags = 0;
  struct addrinfo *result;

  info_result = getaddrinfo(host, service, &(socket->hints), &result);

  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
    return info_result;
  }
  iterate_addrinfo_list(result, process_info_to_connect, &is_connected);
  return 0;
}
