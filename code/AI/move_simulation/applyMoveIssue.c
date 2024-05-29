#include "applyMoveIssue.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index)
{
    // Test pawn move remove, on suppose qu'on peut jouer, et qu'on a deja une fonction pour faire jouer l'ami en arrière
    // If the index != -1, this mean there are ghost pawns killed

    lightnightStrike(g, mem, index);
}

void cancelPawnMoveDeter(Game *g, memory_move_t *mem)
{
    bool iw = g->is_white;

    pawnMoveCancel(g, iw, mem->indMovePawn, mem->left);
    freeMemMove(mem);
}

void promotionIssue(Game *g, memory_move_t *mem, int index)
{

    // mem->pos_potential_foe_from_prom = promotionNGE(g, mem->indMovePawn);
}

void pawnMoveBackIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);
}

void cancelPawnMoveBackDeter(Game *g, memory_move_t *mem)
{
    cancelMoveBack(g, mem->indMovePawn, mem->left);
    freeMemMove(mem);
}

void biDeplIssue(Game *g, int indMovePawn, memory_move_t *mem)
{
    mem->full_pawn_data = biDeplNGE(g, g->is_white, indMovePawn);
}

void ccancelBiDeplDeter(Game *g, memory_move_t *mem)
{
    cancelBidepl(g, mem->indMovePawn, mem->full_pawn_data);
    freeMemMove(mem);
}

void queenDeplIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);
}

void cancelQueenDeplDeter(Game *g, memory_move_t *mem)
{
    
    cancelDeplQueen(g, mem->indMovePawn, mem->chainy, mem->init_coord);
    freeMemMove(mem);
}

void rafleIssue(Game *g, memory_move_t *mem, int index)
{

    lightnightStrike(g, mem, index);
}

void cancelRafleDeter(Game *g, memory_move_t *mem)
{
    
    cancelRafle(g, mem->indMovePawn, mem->init_coord, mem->chainy);
    freeMemMove(mem);
}

void lienAmitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
    lienAmitieNGE(lig, col, g, indPawn);
}

void cancelLienAmitieDeter(Game *g, memory_move_t *mem)
{

    cancelLienAmitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}

void lienEnnemitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
    lienEnnemitieNGE(lig, col, g, indPawn);
}

void cancelLienEnnemitieDeter(Game *g, memory_move_t *mem)
{

    cancelLienEnnemitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}