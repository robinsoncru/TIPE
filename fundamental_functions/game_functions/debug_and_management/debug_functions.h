#ifndef DEBUG_FUNCTIONS
#define  DEBUG_FUNCTIONS

#include "../aux_game_functions/aux_functions.h"

void flush();
// Fait planter le programme en affichant un message d'erreur
// si la condition n'est pas respectee
void assertAndLog(bool condition, char *message);
// Affiche ligne, colonne et coordonnees des cases
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG], Game *g);

// Affiche les caracteristiques des Pions
void print_pawns(Game *g);
void print_pawn(pawn p, int ind);
/* print check in the terminal and flush it */
void error();


#endif //  DEBUG_FUNCTIONS