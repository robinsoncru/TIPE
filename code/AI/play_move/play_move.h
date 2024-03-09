#ifndef PLAYMOVE
#define PLAYMOVE

#include "../move_simulation/move_simulation.h"

memory_move_t *applyDeter(Game *g, Move coup);
void applyMove(Game *g, memory_move_t *mem, int nbIssue);
void applyRecipMove(Game *g, memory_move_t *mem);
void applyRecipDeter(Game *g, memory_move_t *mem);

#endif