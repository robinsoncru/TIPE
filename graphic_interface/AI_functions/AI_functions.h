#ifndef AI_FUNCTIONS
#define AI_FUNCTIONS 

#include "../../fundamental_functions/interface/interface.h"

void pawnMoveAI(Game *g, int indMovePawn, bool left);
void promotionIA(Game *g, int indMovePawn);
void pawnMoveBackAI(Game *g, int indMovePawnBack, bool left);
void biDeplAI(Game *g, int indMovePawn);
void queenDeplAI(Game *g, int indMovePawn, queen_move_t coords);
void rafleAI(Game *g, int indMovePawn);
void lienAmitieAI(Game *g, int indPawn, int lig, int col);
void lienEnnemitieAI(Game *g, int indPawn, int lig, int col);

#endif