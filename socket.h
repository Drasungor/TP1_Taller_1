#ifndef _SOCKET_H_
#define  _SOCKET_H_


typedef struct{

}socket_t;


void socket_init(socket_t *socket);

void socket_release(socket_t *socket);

int socket_bind_and_listen(socket_t *socket, const char *service);

int socket_connect(socket_t *socket, const char *host, const char *service);



#endif
