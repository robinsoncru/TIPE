#ifndef AI_FUNCTIONS
#define AI_FUNCTIONS 

#include "give_issues.h"

void pawnMoveAI(Game *g, memory_move_t *mem, int index);
void promotionAI(Game *g, memory_move_t *mem, int index);
void pawnMoveBackAI(Game *g, memory_move_t *mem, int index);
void rafleAI(Game *g, memory_move_t *mem, int index);
void queenDeplAI(Game *g, memory_move_t *mem, int index);

memory_move_t *biDeplAI(Game *g, int indMovePawn);
memory_move_t *lienAmitieAI(Game *g, int indPawn, int lig, int col);
memory_move_t *lienEnnemitieAI(Game *g, int indPawn, int lig, int col);

void cancelPawnMoveAI(Game *g, memory_move_t *mem);
void cancelPromotionAI(Game *g, memory_move_t *mem);
void cancelPawnMoveBackAI(Game *g, memory_move_t *mem);
void cancelBiDeplAI(Game *g, memory_move_t *mem);
void cancelQueenDeplAI(Game *g, memory_move_t *mem);
void cancelRafleAI(Game *g, memory_move_t *mem);
void cancelLienAmitieAI(Game *g, memory_move_t *mem);
void cancelLienEnnemitieAI(Game *g, memory_move_t *mem);

#endif