#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "remote_sudoku.h"

int main(int argc, char const *argv[]) {
  //VER SI CONVIENE CHEQUEAR DIRECTAMENTE EL NRO PORQUE NO SE ESTÁ
  //TRATANDO DE HACER UN PROCESO QUE QUERÉS VER SI SALIÓ BIEN,
  //ES ALGO CONSTANTE, CONVIENE TRATAR CON LITERALES XQ ASÍ
  //MOSTRAS QUE ES UN TEMA DE MARNEJO DE ERRORES
  if (remote_sudoku_start(argv + 1, argc-1) != 0) {
    return 1;
  }
  return 0;
}
