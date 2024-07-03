#ifndef MOVE_LISTING
#define MOVE_LISTING

#include <stdlib.h>
#include "../../fundamental_functions/game_functions/Source/source_header.h"
#include "../../fundamental_functions/game_functions/Logic/logic_functions.h"
#include "move_max.h"
#include "move_struct/move_struct.h"
#include "rafle_listing/rafle_listing.h"
#include "listing_checks/listing_checks.h"

MoveTab* listMoves(Game* g);
MoveTab *listMovesNoRafleNoPromotion(Game *g);
#endif