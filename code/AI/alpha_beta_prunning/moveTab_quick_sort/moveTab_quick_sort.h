#ifndef MOVETAB_QUICK_SORT
#define MOVETAB_QUICK_SORT

#include "../../move_listing/move_listing.h"
#include "../../../structures_fondamentales/fgame_struct.h"

void moveTabQuickSort(Game* g, MoveTab* t, float (*h)(Game*));

#endif