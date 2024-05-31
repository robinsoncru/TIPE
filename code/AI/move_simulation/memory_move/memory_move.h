#ifndef MEMORY_MOVE
#define MEMORY_MOVE

#include "../../move_listing/move_listing.h"
#include "../../../fundamental_functions/affichage/affichage.h"



memory_move_t *initMemMove(int indMovePawn, moveType type);
void freeMemMove(memory_move_t *mem);

#endif