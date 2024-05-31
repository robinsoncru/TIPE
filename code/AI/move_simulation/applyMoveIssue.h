#ifndef AI_FUNCTIONS
#define AI_FUNCTIONS 

#include "applyMoveDeter.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index);
void promotionIssue(Game *g, memory_move_t *mem, int index);
void pawnMoveBackIssue(Game *g, memory_move_t *mem, int index);
void rafleIssue(Game *g, memory_move_t *mem, int index);
void queenDeplIssue(Game *g, memory_move_t *mem, int index);

void biDeplIssue(Game *g, int indMovePawn, memory_move_t *mem);
void lienAmitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem);
void lienEnnemitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem);


void cancelPawnMoveDeter(Game *g, memory_move_t *mem);
void cancelPromotionDeter(Game *g, memory_move_t *mem);
void cancelPawnMoveBackDeter(Game *g, memory_move_t *mem);
void cancelBiDeplDeter(Game *g, memory_move_t *mem);
void cancelQueenDeplDeter(Game *g, memory_move_t *mem);
void cancelRafleDeter(Game *g, memory_move_t *mem);
void cancelLienAmitieDeter(Game *g, memory_move_t *mem);
void cancelLienEnnemitieDeter(Game *g, memory_move_t *mem);

void cancelSelectedIssue(Game *g, memory_move_t *mem, int index);


#endif