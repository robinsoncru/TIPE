#include "path_tree.h"
#include "rafle_tree_calc.h"
#include "../../fundamental_functions/game_functions_draughts.h"

void eatingTest(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], PathTree* pathTree);
    //Mange les pions dans un ordre arbitraire

int testRafleTree(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);