#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "socket.h"


typedef int (*linking_function_t) (int socket_fd, const struct sockaddr *addr, socklen_t addr_len);

/*
//Iterates the list while the visit function returns true
void iterate_addrinfo_list(struct addrinfo *element, bool (*visit)(struct addrinfo*, void *extra), void *extra){
  while ((element != NULL) && (visit(element, extra))) {
    element = element->ai_next;
  }
}

//Tries to bind an addess.
//Returns true if it fails, false otherwise
bool process_info_to_bind(struct addrinfo* info, void *extra){
  int binding_value = 0;
  bool *is_bounded = (bool*)extra;

  int socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (socket_fd == -1) {
    return true;
  }

}


//Tries to connect a socket to the host.
//Returns true if it fails, false otherwise
bool process_info_to_connect(struct addrinfo* info, void *extra){
  int connect_value = 0;
  bool *is_connected = (bool*)extra;

  int socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  if (socket_fd == -1) {
    return true;
  }
  connect_value = connect(socket_fd, info->ai_addr, info->ai_addrlen);
  if (connect_value == -1) {
    close(socket_fd);
    return true;
  }
  *is_connected = false;
  return false;
}
*/

//Gets a socket and executes the linking function.
//Returns true if it succeeds and false if it fails,
//in which case the socked_fd value must be ignored
bool process_info_to_link(struct addrinfo* info, int *socket_fd, linking_function_t link){
  int link_value = 0;
  bool is_linked = false;

  //VER SI SE PUEDE SACAR LA "ESCALERA DE LLAVES"
  while ((info != NULL) && (!is_linked)) {
    *socket_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
    if (*socket_fd != -1) {
      link_value = link(*socket_fd, info->ai_addr, info->ai_addrlen);
      if (link_value == -1) {
        close(*socket_fd);
      } else{
        is_linked = true;
      }
    }
    info = info->ai_next;
  }
  return is_linked;
}



void socket_init(socket_t *sckt){
  sckt->file_descriptor = 0;
  memset(&(sckt->hints), 0, sizeof(struct addrinfo));
  sckt->hints.ai_family = AF_INET;
  sckt->hints.ai_socktype = SOCK_STREAM;
  sckt->is_client = false;
  sckt->is_server = false;
  sckt->client = 0;
}

void socket_release(socket_t *socket){
}

int socket_bind_and_listen(socket_t *sckt, const char *service){
  int info_result = 0;
  bool is_bounded = false;
  int socket_fd = 0;
  sckt->hints.ai_flags = AI_PASSIVE;
  struct addrinfo *result;

  info_result = getaddrinfo(NULL, service, &(sckt->hints), &result);
  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
    return info_result;
  }
  is_bounded = process_info_to_link(result, &socket_fd, bind);
  freeaddrinfo(result);
  if (!is_bounded) {
    return BOUNDING_ERROR;
  }
  sckt->file_descriptor = socket_fd;
  return SUCCESS;
}

int socket_connect(socket_t *sckt, const char *host, const char *service){
  int info_result = 0;
  bool is_connected = false;
  int socket_fd = 0;
  sckt->hints.ai_flags = 0;
  struct addrinfo *result;

  if (sckt->is_server) {
    return false;
  }
  info_result = getaddrinfo(host, service, &(sckt->hints), &result);
  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
    return info_result;
  }
  is_connected = process_info_to_link(result, &socket_fd, connect);
  freeaddrinfo(result);
  if (!is_connected) {
    return CONNECTION_ERROR;
  }
  sckt->file_descriptor = socket_fd;
  sckt->is_client = true;
  return SUCCESS;
}
