#ifndef DEBUG_FUNCTIONS
#define  DEBUG_FUNCTIONS

#include "../precondition/precondition.h"

// Affiche ligne, colonne et coordonnees des cases
void print_damier(Case **damier, Game *g);

// Affiche les caracteristiques des Pions
void print_pawns(Game *g);
void print_pawn(pawn p, int ind);
void print_little_linked_list(int_chain *l);
void printv(char *s);
void picture_this(Game *g);
void picture_game(Game *g, bool iw);


#endif //  DEBUG_FUNCTIONS