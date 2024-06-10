#ifndef ALPHA_BETA_PRUNNING
#define ALPHA_BETA_PRUNNING

#include "../move_listing/move_listing.h"
#include "../../structures_fondamentales/fgame_struct.h"
#include "../ai_lists/ai_struct.h"
#include "../move_listing/move_listing_debug/print_moves.h"

Move alphaBetaPrunning(Game* g, AI ai);

#endif