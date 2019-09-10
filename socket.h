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


void socket_init(socket_t *sckt);

void socket_release(socket_t *sckt);

//Tries to bind to the service and sets the socket as a server,
//listening to any pending connection
//If it fails the file descriptor value must be ignored and accept
//shouldn't be called
//Returns: 0 it it excecutes succesfully
//         -1 if there is an error
int socket_bind_and_listen(socket_t *sckt, const char *service);


//Accepts a pending connection
//Returns 0 if it's successful and -1 if it fails
//If it didn't bind_and_listen previously returns -3
int socket_accept(socket_t *sckt);

//Connects to the service of the specified host
//Returns 0 if it's successful and -1 if it fails
//if
int socket_connect(socket_t *sckt, const char *host, const char *service);

//Sends len bytes of the buffer received
//It fails if the socket is not connected, didn't
//accept a connection, or due to errors in sending the message
//Returns: 0 if it's successful
//         -1 if there is an error sending the message
//         -2 if the socket that is going to receive the message
//          is closed
//         -3 if the socket didin't accept or connect
int socket_send(socket_t *sckt, const void *buffer, size_t len);


//Receives len bytes and places them in the buffer
//It fails if the socket is not connected or due
//to errors in sending the message
//Returns: 0 if it's successful
//         -1 if there is an error sending the message
//         -2 if the socket that is going to receive the message
//          is closed
//         -3 if the socket didin't accept or connect
int socket_receive(socket_t *sckt, void *buffer, size_t len);

#endif
