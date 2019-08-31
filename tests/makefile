CC = gcc
CFLAGS = -g -std=c99 -Wall -Werror
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes



CFILES = cell_test.c sudoku_test.c testing.c tests.c ../cell.c ../sudoku.c
HFILES = cell_test.h sudoku_test.h testing.h ../cell.h ../sudoku.h
EXEC_S = test

build: $(CFILES)
	$(CC) $(CFLAGS) -o $(EXEC_S) $(CFILES)

run: build
	./$(EXEC_S)

gdb_test: build
	gdb $(GDBFLAGS) ./$(EXEC_S_GREP)

clean:
	rm -f *.o $(EXEC_S)

zip:
	zip $(PADRON).zip $(CFILES) $(HFILES)
