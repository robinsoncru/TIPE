#ifndef RAFLE_LISTING
#define RAFLE_LISTING

#include "../../../rafle/calc/path_tree.h"
#include "../../../rafle/rafle_choice/path_struct.h"
#include "../../../fundamental_functions/game_functions/Source/source_header.h"
#include "../../../fundamental_functions/coord/coord.h"
#include "../move_struct/move_struct.h"
#include "../../../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include "../../../rafle/rafle.h"

void listRafles(Game* g, int selectedPawn, Coord temporaryPos, Move* temporaryResult, int* nbMoves);

#endif