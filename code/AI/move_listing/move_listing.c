#include "move_listing.h"
#include "listing_checks/listing_checks.h"
#include "move_back_listing/backwardMoveTab_interface.h"
#include "move_max.h"
#include "move_struct/move_struct.h"
#include "rafle_listing/rafle_listing.h"
#include "../../fundamental_functions/game_functions/access_functions/access_functions.h"
#include "../../fundamental_functions/game_functions/Logic/logic_functions.h"
#include "move_back_listing/backwardMoveTab_quick_sort/backwardMoveTab_quick_sort.h"
#include "move_counting/move_counting.h"
#include <stddef.h>
#include <stdlib.h>

void listMovesMoveBackAux(Game *g, int *nbMoves, Move *temporaryResult,
                          Move currentMove, backwardMoveTab_t *backTab, int i)
{
    // entree : backTab a contient les indices du groupe d'amis
    // dans un ordre arbitraire
    if (i == backTab->n)
    {
        currentMove.backwardPawnMoves = backwardMoveTabCopy(backTab);
        temporaryResult[*nbMoves] = currentMove;
        *nbMoves = *nbMoves + 1;
    }
    else
    {
        int manipulatedPawn = backwardMoveTabGetIndMovedPawn(backTab, i);
        pawn initialState = get_pawn(g, g->is_white, manipulatedPawn);
        bool canMoveBackRight, canMoveBackLeft;
        int addLine = g->is_white ? -1 : 1;
        canMoveBackLeft = caseIsAccessible(g, g->is_white,
                                           initialState.lig + addLine, initialState.col - 1);
        canMoveBackRight = caseIsAccessible(g, g->is_white,
                                            initialState.lig + addLine, initialState.col + 1);
        if (canMoveBackLeft)
        {
            change_pawn_place(g, manipulatedPawn, g->is_white,
                              initialState.lig + addLine, initialState.col - 1);
            backwardMoveTabSetDir(backTab, i, LEFT);
            listMovesMoveBackAux(g, nbMoves, temporaryResult,
                                 currentMove, backTab, i + 1);
        }
        if (canMoveBackRight)
        {
            change_pawn_place(g, manipulatedPawn, g->is_white,
                              initialState.lig + addLine, initialState.col + 1);
            backwardMoveTabSetDir(backTab, i, RIGHT);
            listMovesMoveBackAux(g, nbMoves, temporaryResult,
                                 currentMove, backTab, i + 1);
        }
        change_pawn_place(g, manipulatedPawn, g->is_white,
                          initialState.lig, initialState.col);
        if (!canMoveBackLeft && !canMoveBackRight)
        {
            backwardMoveTabSetDir(backTab, i, NO_MOVE);
            listMovesMoveBackAux(g, nbMoves, temporaryResult,
                                 currentMove, backTab, i + 1);
        }
    }
}

Move *listMovesMoveBack(Game *g, int *resSize)
{
    Move currentMove;
    int nbMoves = 0;
    currentMove.type = pawnMoveBackType;
    int nbPawns = taille_list(g->inds_move_back);
    Move *temporaryResult = malloc(quickPow(2, nbPawns) * sizeof(Move));

    backwardMoveTab_t *backTab = backwardMoveTabCreate(nbPawns);
    for (int i = 0; i < nbPawns; i++)
    {
        backwardMoveTabSetIndMovedPawn(backTab, i, get(g->inds_move_back, i));
    }
    backwardMoveTabQuickSort(g, backTab);
    listMovesMoveBackAux(g, &nbMoves, temporaryResult,
                         currentMove, backTab, 0);
    if (nbMoves == 0)
    { // si les pions a reculer sont tous bloques
        currentMove.type = passType;
        temporaryResult[0] = currentMove;
        *resSize = 1;
    }
    return temporaryResult;
}

void listMovesBiDepl(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;

    currentMove.type = biDeplType;
    if (canBiDepl(g, selectedPawn, g->is_white))
    {
        temporaryResult[*nbMoves] = currentMove;
        *nbMoves = *nbMoves + 1;
    }
}

void listMovesMovePawn(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;

    currentMove.type = pawnMoveType;
    for (int k = 0; k < 2; k++)
    {
        if (canMove(g, g->is_white, selectedPawn, k != 1))
        {
            currentMove.left = k != 1;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesPromotion(Game *g, Move *temporaryResult, int *nbMoves)
{
    Move currentMove;
    currentMove.type = promotionType;

    for (int k = 0; k < g->nb_pawns[g->is_white]; k++)
    {
        if (canPromotion(g, k, g->is_white))
        {
            currentMove.manipulatedPawn = k;
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesBefriend(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = lienAmitieType;

    for (int k = 0; k < g->nb_pawns[!g->is_white]; k++)
    {
        Coord c = coord_from_ind(g, k, !g->is_white);
        if (canBeFriend(g, selectedPawn, g->is_white, get_case_damier(g, c.i, c.j)))
        {
            // Isfriend pas valide, modif, a verifier
            currentMove.lig = get_pawn_value(g, !g->is_white, k, LIG);
            currentMove.col = get_pawn_value(g, !g->is_white, k, COL);
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesEnnemy(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawn;
    currentMove.type = lienEnnemitieType;
    Coord currentPawnCoordinates;
    for (int k = 0; k < g->nb_pawns[!g->is_white]; k++)
    {
        currentPawnCoordinates = coord_from_ind(g, k, !g->is_white);
        if (canBeEnnemy(g, selectedPawn, g->is_white,
                        get_case_damier(g, currentPawnCoordinates.i, currentPawnCoordinates.j)))
        {
            // IsEnnemiable pas valide
            currentMove.lig = get_pawn_value(g, !g->is_white, k, LIG);
            currentMove.col = get_pawn_value(g, !g->is_white, k, COL);
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
        }
    }
}

void listMovesForGhostPawns(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    listMovesBiDepl(g, selectedPawn, temporaryResult, nbMoves);
    listMovesMovePawn(g, selectedPawn, temporaryResult, nbMoves);
}

void listMovesForQueen(Game *g, int selectedPawnIndex, Move *temporaryResult, int *nbMoves)
{
    // Une reine ne peut pas gagner de nouveau mais seulement garder ses anciens

    pawn selectedPawn = get_pawn(g, g->is_white, selectedPawnIndex);
    Coord selectedPawnPos = {.i = selectedPawn.lig, .j = selectedPawn.col};
    // possible cases where to move for a queen
    Move currentMove;
    currentMove.manipulatedPawn = selectedPawnIndex;
    currentMove.type = queenDeplType;
    currentMove.pos_dame = selectedPawnPos;

    listRafles(g, selectedPawnIndex, selectedPawnPos, temporaryResult, nbMoves);

    int possibleShifts[2] = {-1, 1};
    Coord moveDirection;
    int oldNbMoves;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            moveDirection.i = possibleShifts[i];
            moveDirection.j = possibleShifts[j];
            currentMove.pos_dame = selectedPawnPos;
            for (int k = 1; k < NB_CASE_LG; k++)
            {
                currentMove.pos_dame = add(currentMove.pos_dame, moveDirection);
                // printf("pos pion dame %d %d\n", currentMove.pos_dame.i, currentMove.pos_dame.j);
                flush();
                if (!caseIsAccessible(g, g->is_white,
                                      currentMove.pos_dame.i, currentMove.pos_dame.j))
                {
                    break;
                }
                oldNbMoves = *nbMoves;

                listRafles(g, selectedPawnIndex, currentMove.pos_dame, temporaryResult, nbMoves);

                if (oldNbMoves == *nbMoves)
                { // s'il n'y a aucune rafle a jouer
                    temporaryResult[*nbMoves] = currentMove;
                    *nbMoves = *nbMoves + 1;
                }
            }
        }
    }
}

void listMovesForPawn(Game *g, int selectedPawn, Move *temporaryResult, int *nbMoves)
{
    listMovesBiDepl(g, selectedPawn, temporaryResult, nbMoves);
    listMovesMovePawn(g, selectedPawn, temporaryResult, nbMoves);
    Coord tmpPos = {.i = get_pawn_value(g, g->is_white, selectedPawn, LIG),
                    .j = get_pawn_value(g, g->is_white, selectedPawn, COL)};
    listRafles(g, selectedPawn, tmpPos, temporaryResult, nbMoves);
    if (get_pawn_value(g, g->is_white, selectedPawn, FRIENDLY) == VOID_INDEX && get_pawn_value(g, g->is_white, selectedPawn, ENNEMY) == VOID_INDEX)
    {
        listMovesBefriend(g, selectedPawn, temporaryResult, nbMoves);
        listMovesEnnemy(g, selectedPawn, temporaryResult, nbMoves);
    }
}

void listMovesRafleCount(Move *temporaryResult, int nbMoves, int *rafleCount, int *length)
{
    int nbRafles = 0;
    int maxRafleLength = 0;
    int currentLength;
    Move currentMove;
    for (int k = 0; k < nbMoves; k++)
    {
        currentMove = temporaryResult[k];
        currentLength = (currentMove.type == rafleType) ? pathTreeDepth(currentMove.rafleTree) : -1;
        if (currentLength > maxRafleLength)
        {
            nbRafles = 0;
            maxRafleLength = currentLength;
        }
        if (currentLength == maxRafleLength)
        {
            nbRafles++;
        }
    }

    *rafleCount = nbRafles;
    *length = maxRafleLength;
}

MoveTab *listMovesFilterRafles(Game *g, Move *temporaryResult, int nbMoves)
{
    int rafleCount, length;
    listMovesRafleCount(temporaryResult, nbMoves, &rafleCount, &length);

    MoveTab *res = malloc(sizeof(MoveTab));
    Move currentMove;
    switch (rafleCount)
    {
    case 0:
        res->size = nbMoves;
        res->tab = malloc(nbMoves * sizeof(Move));
        for (int k = 0; k < nbMoves; k++)
        {
            currentMove = temporaryResult[k];
            currentMove.rafle = NULL;
            currentMove.rafleTree = emptyTree;
            res->tab[k] = currentMove;
        }
        break;

    default:
        res->size = 0;
        res->tab = malloc(rafleCount * sizeof(Move));
        for (int k = 0; k < nbMoves; k++)
        {
            currentMove = temporaryResult[k];
            if (currentMove.type == rafleType && pathTreeDepth(currentMove.rafleTree) == length)
            {
                res->tab[res->size] = currentMove;
                res->size++;
            }
        }
        nbMoves = rafleCount;
        break;
    }

    // Dans tous les cas, enregistre les coord du pions
    for (int k = 0; k < nbMoves; k++)
    {
        res->tab[k].coordManipulatedPawn = coord_from_ind(g, res->tab[k].manipulatedPawn, g->is_white);
    }

    free(temporaryResult);
    return res;
}

MoveTab *listMoves(Game *g)
{
    Move *temporaryResult;
    int nbMoves;
    int majoration;
    if (!is_empty(g->inds_move_back))
    {
        temporaryResult = listMovesMoveBack(g, &nbMoves);
    }
    else
    {
        majoration = countMoves(g);
        temporaryResult = malloc(majoration * sizeof(Move));
        nbMoves = 0;
        listMovesPromotion(g, temporaryResult, &nbMoves);

        for (int k = 0; k < g->nb_pawns[g->is_white]; k++)
        {
            if (get_pawn_value(g, g->is_white, k, ALIVE))
            {
                if (get_pawn_value(g, g->is_white, k, QUEEN))
                {
                    listMovesForQueen(g, k, temporaryResult, &nbMoves);
                }
                else if (get_pawn_value(g, g->is_white, k, PBA) != 1)
                {
                    listMovesForGhostPawns(g, k, temporaryResult, &nbMoves);
                }
                else
                {
                    listMovesForPawn(g, k, temporaryResult, &nbMoves);
                }
            }
        }
    }
    assertAndLog(majoration >= nbMoves, "Erreur dans la majoration.");
    return listMovesFilterRafles(g, temporaryResult, nbMoves);
}