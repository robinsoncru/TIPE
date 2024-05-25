#ifndef CALCULUS_FUNCTIONS
#define CALCULUS_FUNCTIONS

#include "../debug_game_functions/debug_functions.h"


// Calculus fonctions
int NON(int b);
int abs(int a);
bool int_to_bool(int integer);
int bool_to_int(bool b);

Coord coord_from_ind(Game *g, int ind, bool color);
int ind_from_coord(Game *g, int lig, int col);

int quickPow(int a, int n);

#endif //CALCULUS FUNCTIONS