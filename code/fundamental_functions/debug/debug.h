#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../../structures_fondamentales/fusion_struct.h"


#ifndef DEBUG
#define DEBUG
// Debug functions
//print functions
/* print check in the terminal and flush it */
void error();
void flush();


//Fait planter le programme en affichant un message d'erreur
//si la condition n'est pas respectee
void assertAndLog(bool condition, char* message);

bool teste_frame(Game *g, bool color, int rec);
#endif //DEBUG