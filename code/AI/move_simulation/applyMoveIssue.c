#include "applyMoveIssue.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index)
{
    // Seule issues possibles différentes: le nuage
    lightnightStrike(g, mem, index);
    mem->had_become_a_queen = endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void promotionIssue(Game *g, memory_move_t *mem, int index)
{

    mem->pos_potential_foe_from_prom = promotionNGE(g, g->is_white, mem->indMovePawn, index);
    Coord pos = mem->pos_potential_foe_from_prom;
    if (pos.i != -1)
    {
        assert(pos.j != -1);
        int indNew = ind_from_coord(g, pos.i, pos.j);
        mem->had_become_a_queen = endTurnGameManagementNGE(g, indNew, IND_BAD_CHOICE, false);
    }
    else
    {
        if (pos.j == IND_GLORY_QUEEN)
        {
            mem->had_become_a_queen = endTurnGameManagementNGE(g, mem->indMovePawn, IND_GLORY_QUEEN, false);
        }
        else
        {
            assert(pos.j == IND_NOTHING_HAPPENED);
            mem->had_become_a_queen = endTurnGameManagementNGE(g, mem->indMovePawn, IND_NOTHING_HAPPENED, false);
        }
    }
}

void pawnMoveBackIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);
    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, true);
}

void biDeplIssue(Game *g, int indMovePawn, memory_move_t *mem)
{
    mem->full_pawn_data = biDeplNGE(g, g->is_white, indMovePawn);

    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void queenDeplIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index);

    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void rafleIssue(Game *g, memory_move_t *mem, int index)
{

    lightnightStrike(g, mem, index);

    mem->had_become_a_queen = endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void lienAmitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{

    mem->lig = lig;
    mem->col = col;
    lienAmitieNGE(lig, col, indPawn, g->is_white, g);
    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void lienEnnemitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{

    mem->lig = lig;
    mem->col = col;
    lienEnnemitieNGE(g->is_white, lig, col, indPawn, g);
    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}



void cancelSelectedIssue(Game *g, memory_move_t *mem)
{
    // On doit replacer le pion utilisé (index) à sa position dans le nuage, puis recrée le nuage avec les
    // positions originales
    bool iw = g->is_white;
    int index_origin = mem->lenghtIssues - 1;
    if (!cis_empty(mem->load_cloud_other))
    {

        assertAndLog(index_origin != VOID_INDEX, "nuage présent mais pas de survivant");
        Coord pos_survivor = mem->issues[index_origin].pos_survivor;
        int pbaSurvivor = mem->issues[index_origin].pba;
        int indNoPopPawn = ind_from_coord(g, pos_survivor.i, pos_survivor.j);
        /* Pion initialement conservé dans le nuage */
        change_pawn_place_coord(g, index_origin, !iw, mem->issues[index_origin].pos_survivor);

        recreateCloud(g, mem->load_cloud_other, indNoPopPawn, pbaSurvivor, !iw);
    }
}