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
  sckt->fd = 0;
  memset(&(sckt->hints), 0, sizeof(struct addrinfo));
  sckt->hints.ai_family = AF_INET;
  sckt->hints.ai_socktype = SOCK_STREAM;
  sckt->is_client = false;
  sckt->is_server = false;
  sckt->client = 0;
  sckt->can_accept = false;
}

void socket_release(socket_t *socket){
}

int socket_bind_and_listen(socket_t *sckt, const char *service){
  if (sckt->is_client) {
    return false;
  }

  int info_result = 0;
  bool is_bound = false;
  int listen_value = 0;
  int socket_fd = 0;
  sckt->hints.ai_flags = AI_PASSIVE;
  struct addrinfo *result;

  info_result = getaddrinfo(NULL, service, &(sckt->hints), &result);
  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
    return info_result;
  }
  is_bound = process_info_to_link(result, &socket_fd, bind);
  freeaddrinfo(result);
  if (!is_bounded) {
    return BINDING_ERROR;
  }
  sckt->fd = socket_fd;
  listen_value = listen(sckt->fd, 1);
  if (listen_value != 0) {
    return LISTEN_ERROR;
  }
  sckt->can_accept = true;
  return SUCCESS;
}

int socket_accept(socket_t *sckt){
  if (!can_accept) {
    return INVALID_ACTION;
  }
  sckt->client_fd = accept(sckt->fd, NULL, NULL);
  if (sckt->client_fd == -1) {
    return ACCEPT_ERROR;
  }
  return SUCCESS;
}

int socket_connect(socket_t *sckt, const char *host, const char *service){
  if (sckt->is_server) {
    return false;
  }

  int info_result = 0;
  bool is_connected = false;
  int socket_fd = 0;
  sckt->hints.ai_flags = 0;
  struct addrinfo *result;

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
  sckt->fd = socket_fd;
  sckt->is_client = true;
  return SUCCESS;
}
