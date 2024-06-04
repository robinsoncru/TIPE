#ifndef GIVE_STRUCT
#define GIVE_STRUCT

#include "memory_move/memory_move.h"

// For tab of issues initialisation
#define CREATE_PROM_TAB 0
#define CREATE_CLOUD_TAB 1
#define CREATE_NOTHING 2
#define CREATE_CLOUD_PROM_TAB 3

void initTabIssue(Game *g, int what_kind_of_creation, memory_move_t *mem);
void initTabIssueColor(Game *g, int what_kind_of_creation, memory_move_t *mem, bool color);
void generateCloudDuePawnMove(Game *g, memory_move_t *mem);

void lightnightStrike(Game *g, memory_move_t *mem, int index);
void lightnightStrikeColor(Game *g, memory_move_t *mem, int index, bool color);
void cancelPromotionDeter(Game *g, memory_move_t *mem);

memory_move_t *pawnMoveDeter(Game *g, int indMovePawn, bool left, moveType type);
memory_move_t *promotionDeter(Game *g, int indPawn, moveType type);
memory_move_t *moveBackDeter(Game *g, moveType type);
memory_move_t *rafleDeter(Game *g, int indMovePawn, PathTree *rafleTree, Path *rafle, moveType type);
memory_move_t *queenDeplDeter(Game *g, int indMovePawn, Coord pos_dame, PathTree *rafleTree, Path *rafle, moveType type);
memory_move_t *initMemMove(int indMovePawn, moveType type);

#endif