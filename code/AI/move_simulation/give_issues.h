#ifndef GIVE_STRUCT
#define GIVE_STRUCT

#include "memory_move/memory_move.h"

// For tab of issues initialisation
#define CREATE_PROM_TAB 0
#define CREATE_CLOUD_TAB 1
#define CREATE_NOTHING 2

void initTabIssue(Game *g, int what_kind_of_creation, memory_move_t *mem);
void signCloudDuePawnMove(Game *g, memory_move_t *mem);

void lightnightStrike(Game *g, memory_move_t *mem, int index);
void cancelSelectedIssue(Game *g, memory_move_t *mem);
void cancelIssuePromotion(Game *g, memory_move_t *mem);

memory_move_t *issuePawnMove(Game *g, int indMovePawn, bool left, moveType type);
memory_move_t *issuePromotion(Game *g, int indPawn, moveType type);
memory_move_t *issueMoveBack(Game *g, int indMovePawnBack, bool left, moveType type);
memory_move_t *issueRafle(Game *g, int indMovePawn, PathTree *rafleTree, Path *rafle, moveType type);
memory_move_t *issueQueenDepl(Game *g, int indMovePawn, queen_move_t coords, PathTree *rafleTree, Path *rafle, moveType type);
memory_move_t *issueDeter(int indMovePawn, moveType type);

#endif