#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#ifndef DEBUG
#define DEBUG
// Debug functions
//print functions
/* print check in the terminal and flush it */
void error();
//Fait planter le programme en affichant un message d'erreur
//si la condition n'est pas respectee
void assertAndLog(bool condition, char* message);
void flush();
#endif //DEBUG