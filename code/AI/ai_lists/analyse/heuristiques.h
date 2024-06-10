#ifndef HEURISTIQUE
#define HEURISTIQUE

#include "../../../fundamental_functions/game_functions/game_functions.h"
#include "calc_proba_trivial/calc_proba_trivial.h"
double heuristique_miam_trivial(Game *g);
double heuristique_border_trivial(Game *g);
double heuristique_border(Game *g);

#endif