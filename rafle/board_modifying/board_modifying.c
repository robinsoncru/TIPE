#include "board_modifying.h"
#include <stdio.h>

void eatRafle(Rafle* rafle, pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG]){
    /*Mange une rafle tout en la détruisant au passage.*/
    if (lengthRafle(rafle) >= 2) {
        coord startPos = peekRafle(rafle);
        int ind = damier[startPos.i][startPos.j].ind_pawn;
        coord prevPos, nextPos;
        int deltaI, deltaJ;

        while (lengthRafle(rafle) >= 2) {
            prevPos = popRafle(rafle);
            nextPos = peekRafle(rafle);
            deltaI = (nextPos.i - prevPos.i) >> 1;
            deltaJ = (nextPos.j - prevPos.j) >> 1;

            nonLoggingChangeForEat(pawns, Npawns, damier, ind, prevPos.i, prevPos.j, deltaI, deltaJ);
        }
        
        destroyRafle(rafle);
    }
}

int eatBestRafle(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind){
    Rafle* rafle = bestRafle(pawns, Npawns, damier, ind);
    printf("\nEating rafle :\n");
    printRafle(rafle);
    if (lengthRafle(rafle) <= 1) {
        return ind;
    }
    eatRafle(rafle, pawns, Npawns, damier);
    return IND_CHANGE_ALLOWED;
}