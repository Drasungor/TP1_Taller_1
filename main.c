#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "remote_sudoku.h"

int main(int argc, char const *argv[]) {
  if (remote_sudoku_start(argv + 1, argc-1) != 0) {
    return 1;
  }
  return 0;
}
