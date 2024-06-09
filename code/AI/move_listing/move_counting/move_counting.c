#include "move_counting.h"
#include "rafle_counting/rafle_counting.h"
#include "../../../fundamental_functions/game_functions/access_functions/access_functions.h"
#include "../../../fundamental_functions/game_functions/Logic/logic_functions.h"
#include "../move_back_listing/backwardMoveTab_quick_sort/backwardMoveTab_quick_sort.h"
#include "../../../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include <stddef.h>
#include <stdlib.h>



int countMovesMoveBackAux(Game *g, backwardMoveTab_t *backTab, int i)
{
    // entree : backTab a contient les indices du groupe d'amis
    // dans un ordre arbitraire
    if (i == backTab->n)
    {
        return 1;
    }
    int res = 0;
    int manipulatedPawn = backwardMoveTabGetIndMovedPawn(backTab, i);
    pawn initialState = get_pawn(g, g->is_white, manipulatedPawn);
    bool canMoveBackRight, canMoveBackLeft;
    int addLine = g->is_white ? -1 : 1;
    canMoveBackLeft = caseIsAccessible(g, g->is_white,
        initialState.lig + addLine, initialState.col - 1);
    canMoveBackRight = caseIsAccessible(g, g->is_white,
        initialState.lig + addLine, initialState.col + 1);
    if (!canMoveBackLeft && !canMoveBackRight)
    {
        return countMovesMoveBackAux(g, backTab, i + 1);
    }
    if (canMoveBackLeft)
    {
        change_pawn_place(g, manipulatedPawn, g->is_white,
            initialState.lig + addLine, initialState.col - 1);
        res += countMovesMoveBackAux(g, backTab, i + 1);
    }
    if (canMoveBackRight)
    {
        change_pawn_place(g, manipulatedPawn, g->is_white,
            initialState.lig + addLine, initialState.col + 1);
        res += countMovesMoveBackAux(g, backTab, i + 1);
    }
    change_pawn_place(g, manipulatedPawn, g->is_white,
        initialState.lig, initialState.col);
    return res;
}

int countMovesMoveBack(Game *g)
{
    int nbPawns = taille_list(g->inds_move_back);
    backwardMoveTab_t *backTab = backwardMoveTabCreate(nbPawns);
    for (int i = 0; i < nbPawns; i++)
    {
        backwardMoveTabSetIndMovedPawn(backTab, i, get(g->inds_move_back, i));
    }
    backwardMoveTabQuickSort(g, backTab);
    int res = countMovesMoveBackAux(g, backTab, 0);
    backwardMoveTabFree(backTab);
    return res;
}

int countMovesPromotion(Game *g)
{
    int res = 0;
    for (int k = 0; k < g->nb_pawns[g->is_white]; k++)
    {
        if (canPromotion(g, k, g->is_white))
        {
            res++;
        }
    }
    return res;
}

int countMovesBefriend(Game *g, int selectedPawn)
{
    int res = 0;
    for (int k = 0; k < g->nb_pawns[!g->is_white]; k++)
    {
        Coord c = coord_from_ind(g, k, !g->is_white);
        if (canBeFriend(g, selectedPawn, g->is_white, get_case_damier(g, c.i, c.j)))
        {
            res++;
        }
    }
    return res;
}

int countMovesEnnemy(Game *g, int selectedPawn)
{
    int res = 0;
    Coord currentPawnCoordinates;
    for (int k = 0; k < g->nb_pawns[!g->is_white]; k++)
    {
        currentPawnCoordinates = coord_from_ind(g, k, !g->is_white);
        if (canBeEnnemy(g, selectedPawn, g->is_white,
            get_case_damier(g, currentPawnCoordinates.i, currentPawnCoordinates.j)))
        {
            res++;
        }
    }
    return res;
}

int countMovesForGhostPawns(Game *g, int selectedPawn)
{
    return 3;
}

int countMovesForQueen(Game *g, int selectedPawnIndex)
{
    int res = 0;
    // Une reine ne peut pas gagner de nouveau mais seulement garder ses anciens

    pawn selectedPawn = get_pawn(g, g->is_white, selectedPawnIndex);
    Coord selectedPawnPos = {.i = selectedPawn.lig, .j = selectedPawn.col};
    // possible cases where to move for a queen

    res += countRafles(g, selectedPawnIndex, selectedPawnPos) + 1;

    int possibleShifts[2] = {-1, 1};
    Coord moveDirection, temporaryPos;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            moveDirection.i = possibleShifts[i];
            moveDirection.j = possibleShifts[j];
            temporaryPos = selectedPawnPos;
            for (int k = 1; k < NB_CASE_LG; k++)
            {
                temporaryPos = add(temporaryPos, moveDirection);
                // printf("pos pion dame %d %d\n", currentMove.pos_dame.i, currentMove.pos_dame.j);
                //flush();
                if (!caseIsAccessible(g, g->is_white,
                    temporaryPos.i, temporaryPos.j))
                {
                    break;
                }
                
                res += countRafles(g, selectedPawnIndex, temporaryPos) + 1;
            }
        }
    }
    return res;
}

int countMovesForPawn(Game *g, int selectedPawn)
{
    int res = 3;
    Coord tmpPos = {.i = get_pawn_value(g, g->is_white, selectedPawn, LIG),
                    .j = get_pawn_value(g, g->is_white, selectedPawn, COL)};
    res += countRafles(g, selectedPawn, tmpPos);
    if (get_pawn_value(g, g->is_white, selectedPawn, FRIENDLY) == VOID_INDEX
        && get_pawn_value(g, g->is_white, selectedPawn, ENNEMY) == VOID_INDEX)
    {
        res += countMovesBefriend(g, selectedPawn) + countMovesEnnemy(g, selectedPawn);
    }
    return res;
}

int countMoves(Game *g)
{
    if (!is_empty(g->inds_move_back))
    {
        return countMovesMoveBack(g);
    }
    int nbMoves = 0;
    nbMoves += countMovesPromotion(g);

    for (int k = 0; k < g->nb_pawns[g->is_white]; k++)
    {
        if (get_pawn_value(g, g->is_white, k, ALIVE))
        {
            if (get_pawn_value(g, g->is_white, k, QUEEN))
            {
                nbMoves += countMovesForQueen(g, k);
            }
            else if (get_pawn_value(g, g->is_white, k, PBA) != 1)
            {
                nbMoves += countMovesForGhostPawns(g, k);
            }
            else
            {
                nbMoves += countMovesForPawn(g, k);
            }
        }
    }

    return nbMoves;
}