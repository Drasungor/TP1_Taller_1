#define _POSIX_C_SOURCE 200112L
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "socket.h"


#define SUCCESS 0
#define CONNECTION_ERROR -1
#define BINDING_ERROR -2
#define LISTEN_ERROR -3
#define INVALID_ACTION -4
#define ACCEPT_ERROR -5
#define COMMUNICATION_ERROR -6
#define CLOSED_SOCKET -7


typedef int (*linking_function_t) (int socket_fd, const struct sockaddr *addr, socklen_t addr_len);




static bool process_info_to_link(struct addrinfo* info, int *socket_fd, linking_function_t link){
  int link_value = 0;
  bool is_linked = false;

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



static int get_fd(socket_t *sckt){
  if (sckt->is_server) {
    return sckt->client_fd;
  }
  return sckt->fd;
}


static void set_hints(struct addrinfo *hints){
  memset(hints, 0, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
}

void socket_init(socket_t *sckt){
  sckt->fd = -1;
  sckt->is_client = false;
  sckt->is_server = false;
  sckt->client_fd = 0;
}

void socket_release(socket_t *sckt){
  if (sckt->is_server) {
    shutdown(sckt->client_fd, SHUT_RDWR);
    close(sckt->client_fd);
  }
  if (sckt->is_server || sckt->is_client) {
    shutdown(sckt->fd, SHUT_RDWR);
    close(sckt->fd);
  }
}

int socket_bind_and_listen(socket_t *sckt, const char *service){
  if (sckt->is_client) {
    return false;
  }

  int info_result = 0;
  bool is_bound = false;
  int listen_value = 0;
  int socket_fd = 0;
  struct addrinfo *result;
  struct addrinfo hints;
  set_hints(&hints);
  hints.ai_flags = AI_PASSIVE;

  info_result = getaddrinfo(NULL, service, &hints, &result);
  if (info_result != 0) {
    return info_result;
  }
  is_bound = process_info_to_link(result, &socket_fd, bind);
  freeaddrinfo(result);
  if (!is_bound) {
    return BINDING_ERROR;
  }
  sckt->fd = socket_fd;
  listen_value = listen(sckt->fd, 1);
  if (listen_value != 0) {
    return LISTEN_ERROR;
  }
  sckt->is_server = true;
  return SUCCESS;
}

int socket_accept(socket_t *sckt){
  if (!sckt->is_server) {
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
  struct addrinfo *result;
  struct addrinfo hints;
  set_hints(&hints);
  hints.ai_flags = 0;

  info_result = getaddrinfo(host, service, &hints, &result);
  if (info_result != 0) {
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

//This function can't be reduced to 15 lines because all the error checking is
//necessary and irreducible, as well as the variables initialization
int socket_send(socket_t *sckt, const void *buffer, size_t len){
  if ((!sckt->is_client) && (!sckt->is_server)) {
    return INVALID_ACTION;
  }
  size_t total_bytes_sent = 0;
  size_t current_bytes_sent = 0;
  int fd = get_fd(sckt);
  const char *current_address = buffer;
  while (total_bytes_sent < len) {
    current_bytes_sent = send(fd, current_address, len - total_bytes_sent, MSG_NOSIGNAL);
    if (current_bytes_sent == 0) {
      return CLOSED_SOCKET;
    }
    if (current_bytes_sent < 0) {
      return COMMUNICATION_ERROR;
    }
    current_address += current_bytes_sent;
    total_bytes_sent += current_bytes_sent;
  }
  return SUCCESS;
}

//This function can't be reduced to 15 lines because all the error checking is
//necessary and irreducible, as well as the variables initialization
int socket_receive(socket_t *sckt, void *buffer, size_t len){
  if ((!sckt->is_client) && (!sckt->is_server)) {
    return INVALID_ACTION;
  }
  int total_bytes_received = 0;
  int current_bytes_received = 0;
  int fd = get_fd(sckt);
  char *current_address = buffer;

  while (total_bytes_received < len) {
    current_bytes_received = recv(fd, current_address, len - total_bytes_received, MSG_NOSIGNAL);
    if (current_bytes_received == 0) {
      return CLOSED_SOCKET;
    }
    if (current_bytes_received < 0) {
      return COMMUNICATION_ERROR;
    }
    current_address += current_bytes_received;
    total_bytes_received += current_bytes_received;
  }
  return SUCCESS;
}
