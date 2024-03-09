#include "move_simulation.h"

void pawnMoveAI(Game *g, memory_move_t *mem, int index)
{
    // Test pawn move remove, on suppose qu'on peut jouer, et qu'on a deja une fonction pour faire jouer l'ami en arrière
    // If the index != -1, this mean there are ghost pawns killed

    lightnightStrike(g, mem, index);

    endTurnGameManagementSimple(g, mem->indMovePawn);
}

void cancelPawnMoveAI(Game *g, memory_move_t *mem)
{
    bool iw = g->is_white;

    pawnMoveCancel(g, iw, mem->indMovePawn, mem->left);
    freeMemMove(mem);
}

void promotionAI(Game *g, memory_move_t *mem, int index)
{

    mem->ind_potential_foe = promotionNGE(g, mem->indMovePawn, mem->issues[index].index);

    endTurnGameManagementSimple(g, mem->indMovePawn);
}

void cancelPromotionAI(Game *g, memory_move_t *mem)
{
    freeMemMove(mem);
}

void pawnMoveBackAI(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);

    endTurnGameManagementSimple(g, mem->indMovePawn);
}

void cancelPawnMoveBackAI(Game *g, memory_move_t *mem)
{
    cancelMoveBack(g, mem->indMovePawn, mem->left);
    freeMemMove(mem);
}

void biDeplAI(Game *g, int indMovePawn, memory_move_t *mem)
{
    mem->full_pawn_data = biDeplNGE(g, g->is_white, indMovePawn);

    // Desecrate

    endTurnGameManagementSimple(g, indMovePawn);
}

void cancelBiDeplAI(Game *g, memory_move_t *mem)
{
    cancelBidepl(g, mem->indMovePawn, mem->full_pawn_data);
    freeMemMove(mem);
}

void queenDeplAI(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);

    endTurnGameManagementSimple(g, mem->indMovePawn);
}

void cancelQueenDeplAI(Game *g, memory_move_t *mem)
{
    
    cancelDeplQueen(g, mem->indMovePawn, mem->chainy, mem->init_coord);
    freeMemMove(mem);
}

void rafleAI(Game *g, memory_move_t *mem, int index)
{

    lightnightStrike(g, mem, index); // Dépassement pile

    endTurnGameManagementSimple(g, mem->indMovePawn);
}

void cancelRafleAI(Game *g, memory_move_t *mem)
{
    
    cancelRafle(g, mem->indMovePawn, mem->init_coord, mem->chainy);
    freeMemMove(mem);
}

void lienAmitieAI(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
    lienAmitieNGE(lig, col, g, indPawn);
    endTurnGameManagementSimple(g, indPawn);
}

void cancelLienAmitieAI(Game *g, memory_move_t *mem)
{

    cancelLienAmitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}

void lienEnnemitieAI(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
    lienEnnemitieNGE(lig, col, g, indPawn);
    endTurnGameManagementSimple(g, indPawn);
}

void cancelLienEnnemitieAI(Game *g, memory_move_t *mem)
{

    cancelLienEnnemitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}