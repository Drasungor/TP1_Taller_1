#ifndef _SOCKET_H_
#define  _SOCKET_H_

#define SUCCESS 0
#define CONNECTION_ERROR -1

typedef struct{
  int file_descriptor;
  //VER SI NO VALE LA PENA GUARDARLO PARA INICIALIZARLO UNA SOLA VEZ aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
  struct addrinfo hints;
}socket_t;


void socket_init(socket_t *socket);

void socket_release(socket_t *socket);

int socket_bind_and_listen(socket_t *socket, const char *service);

int socket_connect(socket_t *socket, const char *host, const char *service);



#endif
