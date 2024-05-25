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
    // currentMove.manipulatedPawn = g->inds_move_back;
    for (int k = 0; k < 2; k++) {
        // if (canMoveBack(g, g->is_white, g->inds_move_back, k != 1)) {
            currentMove.left = k != 1;
            temporaryResult[nbMoves] = currentMove;
            nbMoves++;
        // }
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

    for (int k = 0; k < g->nb_pawns[!g->is_white]; k++) {
        if (isFriendable(g, k, !g->is_white)) {
            currentMove.lig = get_pawn_value(g, !g->is_white, k, LIG);
            currentMove.col = get_pawn_value(g, !g->is_white, k, COL);
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesEnnemy(Game* g, int selectedPawn, Move* temporaryResult, int* nbMoves){
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = lienEnnemitieType;

    for (int k = 0; k < 2 * NB_PAWNS; k++) {
        if (isEnnemiable(g, k, !g->is_white)) {
            currentMove.lig = get_pawn_value(g, !g->is_white, k, LIG);
            currentMove.col = get_pawn_value(g, !g->is_white, k, COL);
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

    if (get_pawn_value(g, g->is_white, selectedPawn, FRIENDLY) == VOID_INDEX) {
        listMovesBefriend(g, selectedPawn, temporaryResult, nbMoves);
    }

    pawn p = get_pawn(g, g->is_white, selectedPawn);
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
    listMovesBiDepl(g, selectedPawn, temporaryResult, nbMoves);
    listMovesMovePawn(g, selectedPawn, temporaryResult, nbMoves);
    Coord tmpPos = {.i = get_pawn_value(g, g->is_white, selectedPawn, LIG), 
    .j = get_pawn_value(g, g->is_white, selectedPawn, COL)};
    listRafles(g, selectedPawn, tmpPos, temporaryResult, nbMoves);
    if (get_pawn_value(g, g->is_white, selectedPawn, FRIENDLY) == VOID_INDEX 
    && get_pawn_value(g, g->is_white, selectedPawn, ENNEMY) == VOID_INDEX) {
        listMovesBefriend(g, selectedPawn, temporaryResult, nbMoves);
        listMovesEnnemy(g, selectedPawn, temporaryResult, nbMoves);
    }
}

void listMovesRafleCount(Move* temporaryResult, int nbMoves, int* rafleCount, int* length){
    int nbRafles = 0;
    int maxRafleLength = 0;
    int currentLength;
    Move currentMove;
    for (int k = 0; k < nbMoves; k++) {
        currentMove = temporaryResult[k];
        currentLength = (currentMove.type == rafleType) ? pathTreeDepth(currentMove.rafleTree) : -1; 
        if (currentLength > maxRafleLength) {
            nbRafles = 1;
            maxRafleLength = currentLength;
        }
        else if (currentLength == maxRafleLength) {
            nbRafles++;
        }
    }

    *rafleCount = nbRafles;
    *length = maxRafleLength;
}

MoveTab* listMovesFilterRafles(Move* temporaryResult, int nbMoves){
    int rafleCount, length;
    listMovesRafleCount(temporaryResult, nbMoves, &rafleCount, &length);

    MoveTab* res = malloc(sizeof(MoveTab));
    Move currentMove;
    switch (rafleCount) {
        case 0:
            res->size = nbMoves;
            res->tab = malloc(nbMoves * sizeof(Move));
            for (int k = 0; k < nbMoves; k++) {
                currentMove = temporaryResult[k];
                currentMove.rafle = NULL;
                currentMove.rafleTree = emptyTree;
                res->tab[k] = currentMove;
            }
            break;
        
        default:
            res->size = 0;
            res->tab = malloc(rafleCount * sizeof(Move));
            for (int k = 0; k < nbMoves; k++) {
                currentMove = temporaryResult[k];
                if (res->tab[k].type == rafleType && pathTreeDepth(currentMove.rafleTree) == length) {
                    res->tab[res->size] = currentMove;
                    res->size++;
                }
            }
            free(temporaryResult);
            break;
    }
    return res;
}

MoveTab* listMoves(Game* g){
    Move* temporaryResult;
    // int nbMoves;
    assertAndLog(false, "pas correcte");
    // if (g->inds_move_back != VOID_INDEX) {
    //     temporaryResult = listMovesMoveBack(g, &nbMoves);
    // }
    // else {
    //     temporaryResult = malloc(maxMoves(g) * sizeof(Move));
    //     nbMoves = 0;
    //     listMovesPromotion(g, temporaryResult, &nbMoves);

    //     for (int k = 0; k < 2 * NB_PAWNS; k++) {
    //         if (get_pawn_value(g, g->is_white, k, ALIVE)) {
    //             if (get_pawn_value(g, g->is_white, k, QUEEN)) {
    //                 listMovesForQueen(g, k, temporaryResult, &nbMoves);
    //             }
    //             else if (get_pawn_value(g, g->is_white, k, PBA) != 1) {
    //                 listMovesForGhostPawns(g, k, temporaryResult, &nbMoves);
    //             }
    //             else {
    //                 listMovesForPawn(g, k, temporaryResult, &nbMoves);
    //             }
    //         }
    //     }
    // }

    
    // return listMovesFilterRafles(temporaryResult, nbMoves);
    return listMovesFilterRafles(temporaryResult, 0);
}