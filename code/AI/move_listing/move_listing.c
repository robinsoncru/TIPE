#include "move_listing.h"
#include "listing_checks/listing_checks.h"
#include "move_max.h"
#include "move_struct/move_struct.h"
#include "rafle_listing/rafle_listing.h"
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

void listMovesBiDepl(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    
    currentMove.type = biDeplType;
    if (canBiDepl(g, selectedPawn, g->is_white)) {
        temporaryResult[*nbMoves] = currentMove;
        *nbMoves = *nbMoves + 1;
    }
}

void listMovesMovePawn(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;

    currentMove.type = pawnMoveType;
    for (int k = 0; k < 2; k++) {
        if (canMove(g, g->is_white, selectedPawn, k!=1)) {
            currentMove.left = k != 1;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesPromotion(Game* g, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = promotionType;

    for (int k = 0; k < 2 * NB_PAWNS; k++) {
        if (isPromotable(g, k, g->is_white)) {
            currentMove.manipulatedPawn = k;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesBefriend(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = lienAmitieType;

    pawn p;
    for (int k = 0; k < 2 * NB_PAWNS; k++) {
        if (isFriendable(g, k, !g->is_white)) {
            p = g->allPawns[!g->is_white][k];
            currentMove.lig = p.lig;
            currentMove.col = p.col;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesEnnemy(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = lienEnnemitieType;

    pawn p;
    for (int k = 0; k < 2 * NB_PAWNS; k++) {
        if (isEnnemiable(g, k, !g->is_white)) {
            p = g->allPawns[!g->is_white][k];
            currentMove.lig = p.lig;
            currentMove.col = p.col;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesForGhostPawns(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    listMovesBiDepl(g, selectedPawn, temporaryResult, nbMoves);
    listMovesMovePawn(g, selectedPawn, temporaryResult, nbMoves);
}

void listMovesForQueen(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    pawn p = g->allPawns[g->is_white][selectedPawn];
    Coord pos = {p.lig, p.col};
    //possible cases where to move for a queen
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = queenDeplType;

    int possibleShifts[2] = {-1, 1};
    Coord dir;
    int oldNbMoves;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            dir.i = possibleShifts[i];
            dir.j = possibleShifts[j];
            for (int k = 1; k < NB_CASE_LG; k++) {
                currentMove.coords.pos_dame = add(pos, dir);
                if (!caseIsAccessible(g, g->is_white, dir.i, dir.j)) {
                    break;
                }
                else {
                    oldNbMoves = *nbMoves;
                    listRafles(g, selectedPawn, currentMove.coords.pos_dame, temporaryResult, nbMoves);
                    if (oldNbMoves == *nbMoves) { //s'il n'y a aucune rafle a jouer
                        temporaryResult[*nbMoves] = currentMove;
                        *nbMoves = *nbMoves + 1;
                    }
                    dir.i += possibleShifts[i];
                    dir.j += possibleShifts[j];
                }
            }
        }
    
    }
}

void listMovesForPawn(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    pawn p = g->allPawns[g->is_white][selectedPawn];
    listMovesBiDepl(g, selectedPawn, temporaryResult, nbMoves);
    listMovesMovePawn(g, selectedPawn, temporaryResult, nbMoves);
    Coord tmpPos = {.i = p.lig, .j = p.col};
    listRafles(g, selectedPawn, tmpPos, temporaryResult, nbMoves);
}

MoveTab* listMoves(Game* g){
    Move* temporaryResult;
    int nbMoves;
    if (g->ind_move_back != VOID_INDEX) {
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