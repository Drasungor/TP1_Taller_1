#ifndef _SOCKET_H_
#define  _SOCKET_H_

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define SUCCESS 0
#define CONNECTION_ERROR -1
#define BINDING_ERROR -2
#define LISTEN_ERROR -3
#define INVALID_ACTION -4
#define ACCEPT_ERROR -5


//VER SI HAY QUE CAMBIAR ATRIBUTOS DEL STRUCT PORQUE PUEDE SER QUE ESTO SEA MUY ASQUEROSO
typedef struct{
  int fd;
  //VER SI NO VALE LA PENA GUARDARLO PARA INICIALIZARLO UNA SOLA VEZ aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
  struct addrinfo hints;
  bool is_client;
  bool is_server;
  bool can_accept;
  int client_fd;
}socket_t;



//AGREGAR COMENTARIOS SOBRE LO QUE HACE CADA FUNCION: IMPORTANTE

void socket_init(socket_t *sckt);

void socket_release(socket_t *sckt);

//If it fails the file descriptor value must be ignored
//AGREGAR MAS COMENTARIOS, VER SI CONVIENE DEJAR FD EN 0 SI FALLA
int socket_bind_and_listen(socket_t *sckt, const char *service);

int socket_accept(socket_t *sckt);

int socket_connect(socket_t *sckt, const char *host, const char *service);

//int socket_send(socket_t *sckt);
bool socket_send(socket_t *sckt, const void *buffer, size_t len);

bool socket_receive(socket_t *sckt, void *buffer, size_t len);

#endif
