#include "remote_sudoku.h"

int main(int argc, char const *argv[]) {
  if (remote_sudoku_execute(argv + 1, argc-1) != 0) {
    return 1;
  }
  return 0;
}
