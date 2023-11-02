#include "calc/rafle_calc.h"
#include "board_modifying/board_modifying.h"

void printBestRafle(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indSerialKiller);
void eatBestRafle(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);