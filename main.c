#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include "socket.h"


int main(int argc, char const *argv[]) {

  //argv: 0: nombre, 1: modo, 2: puerto, 3: mensaje de 4 chars si es cliente

  /*if (argc != 3) {
    printf("Cantidad incorrecta de parametros\n");
  }
  */
  socket_t sckt;

  socket_init(&sckt);
  if (strcmp(argv[1], "server") == 0) {
    if (socket_bind_and_listen(&sckt, argv[2]) != 0) {
      printf("Error en bind and listen\n");
      socket_release(&sckt);
      return 1;
    }
    if (socket_accept(&sckt) != 0) {
      printf("Error en accept\n");
      socket_release(&sckt);
      return 1;
    }
    char buf[30];
    if (!socket_receive(&sckt, buf, 4)) {
      printf("Error en receive\n");
      socket_release(&sckt);
      return 1;
    }
    buf[4] = '\0';
    printf("%s\n", buf);
    if (!socket_send(&sckt, buf, 4)) {
      printf("Error en send\n");
      socket_release(&sckt);
      return 1;
    }
  } else if (strcmp(argv[1], "client") == 0){
    if (socket_connect(&sckt, NULL, argv[2]) != 0) {
      printf("Error en connect\n");
      socket_release(&sckt);
      return 1;
    }
    if (!socket_send(&sckt, argv[3], 4)) {
      printf("Error en send\n");
      socket_release(&sckt);
      return 1;
    }
    char mensaje[5];
    if (!socket_receive(&sckt, mensaje, 4)) {
      printf("Error en receive\n");
      socket_release(&sckt);
      return 1;
    }
    mensaje[4] = '\0';
    printf("%s\n", mensaje);
  }

  socket_release(&sckt);

  return 0;
}
