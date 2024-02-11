#ifndef DEBUG_FUNCTIONS
#define  DEBUG_FUNCTIONS

#include "../Source/source_header.h"

void flush();
// Fait planter le programme en affichant un message d'erreur
// si la condition n'est pas respectee
void assertAndLog(bool condition, char *message);
// Affiche ligne, colonne et coordonnees des cases
void print_damier(Case **damier, Game *g);

// Affiche les caracteristiques des Pions
void print_pawns(Game *g);
void print_pawn(pawn p, int ind);
/* print check in the terminal and flush it */
void error();
void print_little_linked_list(maillon *l);
void printv(char *s);
void picture_this(Game *g);


#endif //  DEBUG_FUNCTIONS