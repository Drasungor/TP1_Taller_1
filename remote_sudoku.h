#ifndef _REMOTE_SUDOKU_H_
#define  _REMOTE_SUDOKU_H_

//Receives the arguments (and number) of the commands that it is supposed
//to execute. If it's a valid command it executes it, otherwise fails and
//prints a message
int remote_sudoku_start(char **arguments, int number_of_arguments);


#endif
