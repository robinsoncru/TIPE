#include <stdbool.h>
#include <stdio.h>
#include "path_tree.h"
#include "../../../fundamental_functions/game_functions_draughts.h"

PathTree* rafleTreeCalc(pawn pawns[],pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indSerialKiller);