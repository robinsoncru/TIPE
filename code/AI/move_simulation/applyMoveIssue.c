#include "applyMoveIssue.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index)
{
    // Seule issues possibles différentes: le nuage
    lightnightStrike(g, mem, index);
    mem->had_become_a_queen = endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void promotionIssue(Game *g, memory_move_t *mem, int index)
{
    if (!mem->prom_need_break_cloud)
    {
        mem->pos_potential_foe_from_prom = promotionNGE(g, index);
    }
    Coord pos = mem->pos_potential_foe_from_prom;
    bool iw = g->is_white;
    if (pos.i != -1)
    {
        assert(pos.j != -1);
        int indNew = ind_from_coord(g, pos);

        // si le pion devient de l'autre couleur et qu'un nuage de pion se trouve autour, il doit éclater
        if (!mem->prom_need_break_cloud && canStormBreaksForTheOthers(g, indNew, !iw))
        {
            initTabIssueColor(g, CREATE_CLOUD_PROM_TAB, mem, iw);
        }
        if (mem->prom_need_break_cloud)
        {
            lightnightStrikeColor(g, mem, index, iw);
        }

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
    assert(false);

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
    lienAmitieNGE(lig, col, indPawn, g->is_white, g);
    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void lienEnnemitieIssue(Game *g, int indPawn, int lig, int col, memory_move_t *mem)
{
    assertAndLog(isPawnValid(g) && canBeEnnemy(g, indPawn, g->is_white, get_case_damier(g, lig, col)),
                 "lienEnnemitieIssue : peut pas etre ennemi");
    lienEnnemitieNGE(g->is_white, lig, col, indPawn, g);
    endTurnGameManagementNGE(g, mem->indMovePawn, IND_CHANGE_ALLOWED, false);
}

void cancelSelectedIssue(Game *g, memory_move_t *mem, int index)
{
    // On doit replacer le pion utilisé (index) à sa position dans le nuage, puis recrée le nuage avec les
    // positions originales
    bool iw = g->is_white;
    if (mem->prom_need_break_cloud)
    {
        iw = !iw;
    }
    int index_origin;
    if (mem->prom_need_break_cloud)
    {
        index_origin = 2; // Dans une promotion, le premier elm de la pile est à l'indice 2
    }
    else
    {
        index_origin = 0;
    } // C'est le premier pion dans la liste (pile), à l'indice 0 qui est déplacé
    if (!cis_empty(mem->load_cloud))
    {

        assertAndLog(index_origin != VOID_INDEX, "nuage présent mais pas de survivant");
        Coord pos_survivor0 = mem->issues[index_origin].pos_survivor; // Position d'origine
        Coord pos_survivor = mem->issues[index].pos_survivor;         // Position où il a été déplacé
        int pbaSurvivor = mem->issues[index_origin].pba;
        int indNoPopPawn = ind_from_coord(g, pos_survivor);
        assertAndLog(indNoPopPawn != VOID_INDEX, "Cancel selected Issue : Le pion non supprime est indice pas valide");
        /* Pion initialement conservé dans le nuage */
        change_pawn_place_coord(g, indNoPopPawn, !iw, pos_survivor0);

        recreateCloud(g, mem->load_cloud, indNoPopPawn, pbaSurvivor, !iw);

        // print_int_chain(g->cloud[!iw]);
    }
}