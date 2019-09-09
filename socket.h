#ifndef _SOCKET_H_
#define  _SOCKET_H_

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>



//all attributes are private
typedef struct{
  int fd;
  bool is_client;
  bool is_server;
  int client_fd;
}socket_t;



//AGREGAR COMENTARIOS SOBRE LO QUE HACE CADA FUNCION: IMPORTANTE

void socket_init(socket_t *sckt);

void socket_release(socket_t *sckt);

//Tries to bind to the service and sets the socket as a server,
//listening to any pending connection
//If it fails the file descriptor value must be ignored
int socket_bind_and_listen(socket_t *sckt, const char *service);

//Accepts a pending connection
int socket_accept(socket_t *sckt);

//Connects to the service of the specified host
int socket_connect(socket_t *sckt, const char *host, const char *service);

//Sends len bytes of the buffer received
//It fails if the socket is not connected or did not accept a connection,
//
int socket_send(socket_t *sckt, const void *buffer, size_t len);

//Receives len bytes and places them in the buffer
int socket_receive(socket_t *sckt, void *buffer, size_t len);

#endif
