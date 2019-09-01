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



int get_fd(socket_t *sckt){
  if (sckt->is_server) {
    return sckt->client_fd;
  }
  return sckt->fd;
}


void socket_init(socket_t *sckt){
  sckt->fd = 0;
  memset(&(sckt->hints), 0, sizeof(struct addrinfo));
  sckt->hints.ai_family = AF_INET;
  sckt->hints.ai_socktype = SOCK_STREAM;
  sckt->is_client = false;
  sckt->is_server = false;
  sckt->client_fd = 0;
  sckt->can_accept = false;
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
  sckt->hints.ai_flags = AI_PASSIVE;
  struct addrinfo *result;

  info_result = getaddrinfo(NULL, service, &(sckt->hints), &result);
  if (info_result != 0) {
    //VER SI HAY QUE IMPRIMIR UN MENSAJE DE ERROR
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
  sckt->can_accept = true;
  return SUCCESS;
}

int socket_accept(socket_t *sckt){
  if (!sckt->can_accept) {
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


//PREGUNTA: CONVIENE USAR ESTA IMPLEMENTACION DE SEND?
/*
int soket_send(socket_t *sckt, const void *buffer, size_t element_len, void (*convert_endian)(void*, void*)){

  return htonl(3);
}
*/


//PREGUNTA: CONVIENE HACER QUE ENVIE UN ARRAY DE DATOS EN VEZ DEUN SOLO CONJUNTO DE DATOS?
//VER SI CONVIENE QUE DEVUELVA UN INT (TAL VEZ SE DEVUELVE INT POR SI SE
//AGREGAN DESPUÉS OTROS TIPOS DE ERRORES QUE SE QUIERAN DEVOLVER)
bool soket_send(socket_t *sckt, const void *buffer, size_t len){
  size_t total_bytes_sent = 0;
  size_t current_bytes_sent = 0;
  int fd = get_fd(sckt);
  const char *current_address = buffer;
  //VER SI SACO EL RETURN DEL WHILE POR SI QUEDA MUY MAL

  while (total_bytes_sent < len) {
    current_bytes_sent = send(fd, current_address, len - total_bytes_sent, MSG_NOSIGNAL);
    if (current_bytes_sent < 1) {
      return false;
    }
    current_address += current_bytes_sent;
    total_bytes_sent += current_bytes_sent;
  }
  return true;
}

//VER SI SE PUEDE COMBINAR EN UNA UNICA FUNCION QUE RECIBA
//Y QUE ENVIE, HAY PROBLEMA CON QUE UN BUFFER ES CONST Y EL OTRO NO
bool soket_receive(socket_t *sckt, void *buffer, size_t len){
  int total_bytes_received = 0;
  int current_bytes_received = 0;
  int fd = get_fd(sckt);
  char *current_address = buffer;

  while (current_bytes_received < len) {
    current_bytes_received = recv(fd, current_address, len - current_bytes_received, MSG_NOSIGNAL);
    if (current_bytes_received < 1) {
      return false;
    }
    current_address += current_bytes_received;
    total_bytes_received += current_bytes_received;
  }
  return true;
}
