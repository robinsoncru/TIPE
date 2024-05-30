#include "applyMoveIssue.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index)
{
    // Seule issues possibles différentes: le nuage
    lightnightStrike(g, mem, index);
}

void promotionIssue(Game *g, memory_move_t *mem, int index)
{

    mem->pos_potential_foe_from_prom = promotionNGE(g, g->is_white, mem->indMovePawn, index);
}

void pawnMoveBackIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);
}

void biDeplIssue(Game *g, int indMovePawn, memory_move_t *mem)
{
    mem->full_pawn_data = biDeplNGE(g, g->is_white, indMovePawn);
}

void queenDeplIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);
}

void rafleIssue(Game *g, memory_move_t *mem, int index)
{

    lightnightStrike(g, mem, index);
}

void lienAmitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
   lienAmitieNGE(lig, col, indPawn, g->is_white, g);
}

void lienEnnemitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;

    mem->lig = lig;
    mem->col = col;
   lienEnnemitieNGE(g->is_white, lig, col, indPawn, g);
}