#include "board_modifying.h"
#include <stdio.h>

void eatRafle(Rafle* rafle, pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG]){
    /*Mange une rafle tout en la détruisant au passage.*/
    if (isEmpty(rafle)) {
        printf("Rafle vide\n");
    }
    else {
        coord startPos = peekRafle(rafle);
        int ind = damier[startPos.i][startPos.j].ind_pawn;
        coord prevPos, nextPos;
        int deltaI, deltaJ;

        while (lengthRafle(rafle) >= 2) {
            prevPos = popRafle(rafle);
            nextPos = peekRafle(rafle);
            deltaI = nextPos.i - prevPos.i;
            deltaJ = nextPos.j - prevPos.j;

            nonLoggingChangeForEat(pawns, Npawns, damier, ind, prevPos.i, prevPos.j, deltaI, deltaJ);
            printf("(di, dj) = (%d, %d)", deltaI, deltaJ);
        }

        destroyRafle(rafle);
    }
}

void eatBestRafle(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind){
    Rafle* rafle = bestRafle(pawns, Npawns, damier, ind);
    printf("Eating rafle :\n");
    printRafle(rafle);
    eatRafle(rafle, pawns, Npawns, damier);
}