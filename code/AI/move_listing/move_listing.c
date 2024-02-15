#include "move_listing.h"
#include "move_max.h"
#include <stdlib.h>

Move* listMovesMoveBack(Game* g, int* resSize){
    Move* temporaryResult = malloc(2 * sizeof(Move));
    Move currentMove;
    int nbMoves = 0;

    currentMove.type = pawnMoveBackType;
    currentMove.manipulatedPawn = g->ind_move_back;
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

void listMovesForGhostPawns(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    
    currentMove.type = biDeplType;
    if (canBiDepl(g, selectedPawn, g->is_white)) {
        temporaryResult[*nbMoves] = currentMove;
        *nbMoves = *nbMoves + 1;
    }

    currentMove.type = pawnMoveType;
    for (int k = 0; k < 2; k++) {
        if (canMove(g, g->is_white, selectedPawn, k!=1)) {
            currentMove.left = k != 1;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesForQueen(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    //possible cases where to move for a queen
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;

    currentMove.type = queenDeplType;
    int possibleShifts[2] = {-1, 1};
    Coord dir;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            dir.i = possibleShifts[i];
            dir.j = possibleShifts[j];
            for (int k = 1; k < NB_CASE_LG; k++) {
                currentMove.coords.pos_dame = dir;
                if (caseIsAccessible(g, g->is_white, dir.i, dir.j)) {
                    temporaryResult[*nbMoves] = currentMove;
                    *nbMoves = *nbMoves + 1;
                    dir.i += possibleShifts[i];
                    dir.j += possibleShifts[j];
                }
                else {
                    break;
                }
            }
        }
    
    }

    currentMove.type = 
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