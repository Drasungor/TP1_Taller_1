#ifndef _CLIENT_H_
#define  _CLIENT_H_

#define GET_INDICATOR 'G'
#define PUT_INDICATOR 'P'
#define VERIFY_INDICATOR 'V'
#define RESET_INDICATOR 'R'
#define GET_COMMAND "get"
#define PUT_COMMAND "put"
#define VERIFY_COMMAND "verify"
#define RESET_COMMAND "reset"
#define EXIT_COMMAND "exit"

#define SUCCESS 0
#define SOCKET_ERROR -1
#define MEMORY_ERROR -2
#define INVALID_COMMAND -3

typedef struct{
  socket_t sckt;
}client_t;


int client_init(client_t *client, const char *host, const char *service);

void client_release(client_t *client);

void client_operate(client_t *client);

#endif
