#ifndef ALPHA_BETA_ARG_STRUCT
#define ALPHA_BETA_ARG_STRUCT

#include "../ai_lists/ai_struct.h"
#include "../../structures_fondamentales/fgame_struct.h"

typedef struct alphaBetaArg {
    int depth;
    AI ai;
    Game* g;
    float alpha, beta;
} alphaBetaArg;

#endif