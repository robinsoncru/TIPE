#include "move_listing.h"
#include "move_max.h"
#include <stdlib.h>

Move* listMovesMoveBack(Game* g, int* resSize){
    pawn pawnToMove = g->allPawns[g->is_white][g->ind_move_back];
    int i = pawnToMove.lig, j = pawnToMove.col;
    int perspective = (g->is_white) ? 1 : -1;

    Move* temporaryResult = malloc(2 * sizeof(Move));
    Move currentMove;
    int nbMoves = 0;

    currentMove.type = pawnMoveBackType;
    currentMove.manipulatedPawn = g->ind_move_back;

    int colShift[2] = {-1, 1};
    int newLig = i - perspective;
    for (int k = 0; k < 2; k++) {
        if (canMoveBack(g, g->is_white, g->ind_move_back, k != 1)) {
            currentMove.left = k != 1;
            temporaryResult[nbMoves] = currentMove;
            nbMoves++;
        }
    }

    *resSize = nbMoves;
    return temporaryResult;
}

MoveTab* listMoves(Game* g){
    Move* temporaryResult;
    int nbMoves;
    if (g->ind_move_back == VOID_INDEX) {
        temporaryResult = listMovesMoveBack(g, &nbMoves);
    }
    else {
        temporaryResult = malloc(maxMoves(g) * sizeof(Move));
        nbMoves = 0;
    }

    MoveTab* actualResult = malloc(sizeof(MoveTab));
    actualResult -> size = nbMoves;
    for (int k = 0; k < nbMoves; k++) {
        actualResult->tab[k] = temporaryResult[k];
    }
    free(temporaryResult);
    return actualResult;
}