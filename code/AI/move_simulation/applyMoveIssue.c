#include "applyMoveIssue.h"

void pawnMoveIssue(Game *g, memory_move_t *mem, int index)
{
    // Seule issues possibles différentes: le nuage
    lightnightStrike(g, mem, index, mem->is_white);
    int ind = ind_from_coord(g, mem->movePawn.coord);
    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void promotionIssue(Game *g, memory_move_t *mem, int index)
{

    bool iw = mem->is_white;
    if (!mem->prom_need_break_cloud)
    {
        mem->pos_potential_foe_from_prom = promotionNGE(g, index, iw);
    }
    Coord pos = mem->pos_potential_foe_from_prom;
    int ind = ind_from_coord(g, mem->movePawn.coord);

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
            lightnightStrike(g, mem, index, !iw);
            // Attention, issue on inverse le role des blancs et noirs puisque c'est le pion noir qui peut détruire
            // le nuage blanc puis on lieu les rétroactions
        }

        endTurnGameManagementNGE(g, indNew, IND_BAD_CHOICE, false, mem);
    }
    else
    {
        if (pos.j == IND_GLORY_QUEEN)
        {
            endTurnGameManagementNGE(g, ind, IND_GLORY_QUEEN, false, mem);
        }
        else
        {
            assert(pos.j == IND_NOTHING_HAPPENED);
            endTurnGameManagementNGE(g, ind, IND_NOTHING_HAPPENED, false, mem);
        }
    }
}

void pawnMoveBackIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index, mem->is_white);
    int ind = ind_from_coord(g, mem->movePawn.coord);

    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, true, mem);
}

void biDeplIssue(Game *g, int index, memory_move_t *mem)
{
    int ind;
    if (!mem->is_deter)
    {
        lightnightStrikeDueBiDepl(g, mem, index);
        ind = ind_from_coord(g, mem->issues[index].pos_survivor);
    }
    else
    {
        ind = ind_from_coord(g, mem->movePawn.coord);
    } // rien dans nuage

    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void queenDeplIssue(Game *g, memory_move_t *mem, int index)
{
    lightnightStrike(g, mem, index, mem->is_white);
    int ind = ind_from_coord(g, mem->movePawn.coord);

    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void rafleIssue(Game *g, memory_move_t *mem, int index)
{

    lightnightStrike(g, mem, index, mem->is_white);
    int ind = ind_from_coord(g, mem->movePawn.coord);

    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void lienAmitieIssue(Game *g, Coord posPawnWhichNeedAFriend, int lig, int col, memory_move_t *mem)
{
    int ind = ind_from_coord(g, mem->movePawn.coord);
    int indPawn = ind_from_coord(g, posPawnWhichNeedAFriend);
    lienAmitieNGE(lig, col, indPawn, mem->is_white, g);
    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void lienEnnemitieIssue(Game *g, Coord posPawnWhichNeedAFoe, int lig, int col, memory_move_t *mem)
{
    int ind = ind_from_coord(g, mem->movePawn.coord);
int indPawn = ind_from_coord(g, posPawnWhichNeedAFoe);
    lienEnnemitieNGE(mem->is_white, lig, col, indPawn, g);
    endTurnGameManagementNGE(g, ind, IND_CHANGE_ALLOWED, false, mem);
}

void unloadAndRecreateCloud(Game *g, cloud_chain *load, bool color, memory_move_t *mem, Coord pos_survivor_or, Coord pos_survivor, int pbaSurvivor)
{
    if (!cis_empty(load))
    {
        assertAndLog(validCoord(pos_survivor_or) && validCoord(pos_survivor), "unloadandrecreatecloud : Les positions sont hors jeu");
        int indNoPopPawn = ind_from_coord(g, pos_survivor);
        assertAndLog(indNoPopPawn != VOID_INDEX, "Cancel selected Issue : Le pion non supprime est indice pas valide");
        /* Pion initialement conservé dans le nuage */
        change_pawn_place_coord(g, indNoPopPawn, color, pos_survivor_or);

        recreateCloud(g, load, indNoPopPawn, pbaSurvivor, color);

        // print_int_chain(g->cloud[!iw]);
    }
}

void cancelSelectedIssue(Game *g, memory_move_t *mem, int index)
{
    // On doit replacer le pion utilisé (index) à sa position dans le nuage, puis recrée le nuage avec les
    // positions originales
    bool iw = mem->is_white;
    int index_origin;
    if (mem->type == promotionType) {
        int ind = ind_from_coord(g, mem->movePawn.coord);
        cancelPromotion(g, ind, mem->pos_potential_foe_from_prom, mem->is_white);
    }
    if (mem->prom_need_break_cloud)
    {
        index_origin = 2;                                             // Dans une promotion, le premier elm de la pile est à l'indice 2
        Coord pos_survivor0 = mem->issues[index_origin].pos_survivor; // Position d'origine
        Coord pos_survivor = mem->issues[index].pos_survivor;         // Position où il a été déplacé
        int pbaSurvivor = mem->issues[index_origin].pba;
        unloadAndRecreateCloud(g, mem->load_cloud_same_color, !iw, mem, mem->pawnCloudSameColor.coord,
                               mem->pawnCloudSameColor.coord, mem->pawnCloudSameColor.old_pba);
        unloadAndRecreateCloud(g, mem->load_cloud_other, iw, mem, pos_survivor0, pos_survivor, pbaSurvivor);
    }
    else
    {
        index_origin = 0;
        Coord pos_survivor0 = mem->issues[index_origin].pos_survivor; // Position d'origine
        Coord pos_survivor = mem->issues[index].pos_survivor;         // Position où il a été déplacé
        int pbaSurvivor = mem->issues[index_origin].pba;
        unloadAndRecreateCloud(g, mem->load_cloud_same_color, iw, mem, mem->pawnCloudSameColor.coord,
                               mem->pawnCloudSameColor.coord, mem->pawnCloudSameColor.old_pba);
        unloadAndRecreateCloud(g, mem->load_cloud_other, !iw, mem, pos_survivor0, pos_survivor, pbaSurvivor);
    } // C'est le premier pion dans la liste (pile), à l'indice 0 qui est déplacé
    mem->pawnCloudSameColor = init_cloud_queen();
}

void cancelSelectedIssueBiDepl(Game *g, memory_move_t *mem, int index)
{
    // On doit replacer le pion utilisé (index) à sa position dans le nuage, puis recrée le nuage avec les
    // positions originales
    bool iw = mem->is_white;
    int index_origin = 0;
    Coord pos_survivor0 = mem->issues[index_origin].pos_survivor; // Position d'origine
    Coord pos_survivor = mem->issues[index].pos_survivor;         // Position où il a été déplacé
    int pbaSurvivor = mem->issues[index_origin].pba;
    unloadAndRecreateCloud(g, mem->load_cloud_other, !iw, mem, mem->pawnCloudOtherColor.coord,
                           mem->pawnCloudOtherColor.coord, mem->pawnCloudOtherColor.old_pba);
    unloadAndRecreateCloud(g, mem->load_cloud_same_color, iw, mem, pos_survivor0, pos_survivor, pbaSurvivor);
    // C'est le premier pion dans la liste (pile), à l'indice 0 qui est déplacé
    mem->pawnCloudOtherColor = init_cloud_queen();
}