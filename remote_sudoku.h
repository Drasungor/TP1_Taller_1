#ifndef _REMOTE_SUDOKU_H_
#define  _REMOTE_SUDOKU_H_


#define ERROR -1

//Receives the arguments (and number) of the commands that it is supposed
//to execute. If it's a valid command it executes it, otherwise fails and
//prints a message
//The array or arguments must have the following order:
//Server mode: mode, port
//Client mode: mode, host, port
//Returns 0 on success, -1 on error
int remote_sudoku_start(const char **arguments, int number_of_arguments);


#endif
