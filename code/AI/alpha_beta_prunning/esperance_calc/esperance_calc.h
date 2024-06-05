#ifndef ESPERANCE_CALC
#define ESPERANCE_CALC

#include "../alpha_beta_arg_struct.h"
#include "../../move_listing/move_listing.h"

float esperanceAlphaBetaPrunning(float (*f)(alphaBetaArg), alphaBetaArg alphaBetaArg, Move move);
//retourne l'esperance des appels reccusrsifs
//de l'elagage alpha-beta

float esperanceHeuristique(float (*h)(Game*), Game* g, Move move);
//score moyen du coup move selon l'heuristique h
#endif