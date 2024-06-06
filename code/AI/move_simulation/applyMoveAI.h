#ifndef PLAYMOVE
#define PLAYMOVE

#include "applyMoveIssue.h"
#include "../ai_lists/analyse/heuristiques.h"

memory_move_t *applyDeter(Game *g, Move coup, bool shouldFreeMove);
void applyIssue(Game *g, memory_move_t *mem, int nbIssue);
void applyRecipIssue(Game *g, memory_move_t *mem, int index);
void applyRecipDeter(Game *g, memory_move_t *mem);

#endif