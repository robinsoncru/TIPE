#include "applyMoveDeter.h"

void initTabIssueColor(Game *g, int what_kind_of_creation, memory_move_t *mem, bool color)
{
    // Appeler pour les fonctions non déterministe
    // La couleur appelée est celle de l'adversaire
    int_chain *l = NULL;
    int taille_l;

    Coord c_init = {.i = -1, .j = -1};
    switch (what_kind_of_creation)
    {
    case CREATE_CLOUD_TAB:
        mem->lenghtIssues = g->lengthCloud[color];
        mem->issues = malloc(mem->lenghtIssues * sizeof(issue_t));

        l = g->cloud[color];
        taille_l = taille_list(l); // En pratique, taille_list est recalculé à chaque fois dans la
        // boucle for, d'où le fait de nommer cette variable
        for (int i = 0; i < taille_l; i++)
        {
            int ind = get(l, i);
            int pba_inv = get_pawn_value(g, color, ind, PBA);
            mem->issues[i].pba = pba_inv; // La proba est celle d'un pion fantome
            mem->issues[i].pos_survivor = give_coord(g, color, ind);
            mem->issues[i].pba_promotion = -1;
        }
        break;

    case CREATE_PROM_TAB:

        mem->lenghtIssues = 3;
        mem->issues = malloc(3 * sizeof(issue_t));
        for (int i = 0; i < 3; i++)
        {
            mem->issues[i].pba_promotion = 3;
            mem->issues[i].pba = -1;
            mem->issues[i].pos_survivor = c_init;
        }

        break;

    case CREATE_CLOUD_PROM_TAB:
        assertAndLog(mem->lenghtIssues == 3 && mem->issues != NULL, "initTabIssue : Il n'y pas de promotion");

        l = g->cloud[color];
        taille_l = taille_list(l);
        mem->lenghtIssues += (taille_l - 1);
        // On joue directement une config du nuage lors après l'initialisation
        free(mem->issues);
        mem->issues = malloc(mem->lenghtIssues * sizeof(issue_t));
        // Les deux premières cases du tableau sont laissées vides
        for (int i = 0; i < 2; i++)
        {
            mem->issues[i].pba_promotion = 3;
            mem->issues[i].pba = -1;
            mem->issues[i].pos_survivor = c_init;
        }
        for (int i = 2; i < mem->lenghtIssues; i++)
        {
            int ind = get(l, i - 2);
            int pba_inv = get_pawn_value(g, color, ind, PBA);
            mem->issues[i].pba = pba_inv;
            mem->issues[i].pos_survivor = give_coord(g, color, ind);
            mem->issues[i].pba_promotion = 3;
        }
        mem->prom_need_break_cloud = true;
        break;
    }

    // printf("init mem\n");
    // print_issue(mem->issues, mem->lenghtIssues);
    mem->is_deter = false;
}

void initTabIssue(Game *g, int what_kind_of_creation, memory_move_t *mem)
{
    initTabIssueColor(g, what_kind_of_creation, mem, !g->is_white);
}

void generateCloudDuePawnMove(Game *g, memory_move_t *mem)
{
    bool is_white = g->is_white;
    if (canStormBreaksForTheOthers(g, mem->indMovePawn, is_white))
    {
        initTabIssue(g, CREATE_CLOUD_TAB, mem);
    }
}

void handlePromSurvivorPawns(Game *g, memory_move_t *mem, bool color, int indEnnSurv, int indSameColorSurv, int pbaEnn, int pba)
{
    if (isValidIndexInGame(g, indEnnSurv, !color))
    {
        if (canBePromoted(g, !color, indEnnSurv))
        {
            promote(g, !color, indEnnSurv);
            mem->pawnCloudOtherColor.had_become_queen = true;
        }
        mem->pawnCloudOtherColor.coord = coord_from_ind(g, indEnnSurv, !color);
        mem->pawnCloudOtherColor.old_pba = pbaEnn;
    }

    if (isValidIndexInGame(g, indSameColorSurv, color))
    {
        if (canBePromoted(g, color, indSameColorSurv))
        {
            promote(g, color, indSameColorSurv);
            mem->pawnCloudSameColor.had_become_queen = true;
        }
        mem->pawnCloudSameColor.coord = coord_from_ind(g, indSameColorSurv, color);
        mem->pawnCloudSameColor.old_pba = pba;
    }
}

void lightnightStrike(Game *g, memory_move_t *mem, int index, bool color)
{
    // Eclate le nuage adverse si necessaire, index décide de la position du survivant
    Coord pos_surv_enn = mem->issues[index].pos_survivor;
    int pba = -1;
    int indEnnSurv = stormBreaksNGE(g, !color, mem->load_cloud_other, pos_surv_enn);
    int indSurvSameColor = VOID_INDEX;
    if (canStormBreaksForTheOthers(g, indEnnSurv, !color))
    {
        assertAndLog(cis_empty(mem->load_cloud_same_color), "lightnightstrike charg du friend cloud_chain non vide");
        Coord pos_surv = giveCoordLastInCloud(g, color);
        indSurvSameColor = ind_from_coord(g, pos_surv);
        pba = get_pawn_value(g, color, indSurvSameColor, PBA);
        stormBreaksNGE(g, color, mem->load_cloud_same_color, pos_surv);
    }
    handlePromSurvivorPawns(g, mem, color, indEnnSurv, indSurvSameColor, -1, pba);
    // Pba enn deja enregistré dans issue
}

void lightnightStrikeDueBiDepl(Game *g, memory_move_t *mem, int index)
{
    bool color = mem->is_white;
    Coord pos_surv = mem->issues[index].pos_survivor;
    int indSurv = stormBreaksNGE(g, color, mem->load_cloud_same_color, pos_surv);
    // est ce que indSurv peut niquer l'autre nuage
    int indSurvEnn = VOID_INDEX;
    int pbaEnn = -1;
    if (canStormBreaksForTheOthers(g, indSurv, color))
    {
        assertAndLog(cis_empty(mem->load_cloud_other), "lightnightstrikedubidepl charg du friend cloud_chain non vide");
        Coord pos_surv_enn = giveCoordLastInCloud(g, !color);
        indSurvEnn = ind_from_coord(g, pos_surv_enn);
        pbaEnn = get_pawn_value(g, !color, indSurvEnn, PBA);
        stormBreaksNGE(g, !color, mem->load_cloud_other, pos_surv_enn);
    }
    handlePromSurvivorPawns(g, mem, color, indSurvEnn, indSurv, pbaEnn, -1);
    // La pbaSameColor est deja enregistré dans issues
}

memory_move_t *pawnMoveDeter(Game *g, int indMovePawn, bool left, moveType type)
{
    // Test pawn move remove, on suppose qu'on peut jouer, et qu'on a deja une fonction pour faire jouer l'ami en arrière

    bool iw = g->is_white;
    pawnMoveNGE(g, iw, indMovePawn, left);
    // Attention l'indice de renvoie du friend est dans le structure g

    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->left = left;

    generateCloudDuePawnMove(g, mem);

    return mem;
}

memory_move_t *promotionDeter(Game *g, int indPawn, moveType type)
{

    memory_move_t *mem = initMemMove(indPawn, type);
    initTabIssue(g, CREATE_PROM_TAB, mem);
    return mem;
}

memory_move_t *moveBackDeter(Game *g, moveType type, backwardMoveTab_t *t_backs)
{
    memory_move_t *mem = initMemMove(VOID_INDEX, type);
    extractFriendsFromMove(mem, t_backs);
    // On suppose que le move back est faisable
    moveBackNGE(g, true, false, zero_fun, mem);
    int tailleAmis = taille_list(g->inds_move_back);
    for (int i = 0; i < tailleAmis; i++)
    {
        mem->indMovePawn = pop(g->inds_move_back);
        generateCloudDuePawnMove(g, mem);
        if (mem->lenghtIssues > 1)
        {
            /* Si on a plus de deux issues, c'est qu'on est passé de 1 à pls >= 2 et donc qu'on a un éclatement
            de nuage à gerer, car il y a forcément au moins plus de 2 pions fantomes */
            break; // Le nuage implose, il n'existe plus dès qu'un pion s'en approche trop
        }
    }
    // mem->inds_move_back est caduc, on le libère
    freeIntChain(mem->inds_move_back);
    mem->inds_move_back = NULL;

    return mem;
}

memory_move_t *rafleDeter(Game *g, int indMovePawn, PathTree *rafleTree, Path *rafle, moveType type, Coord pos_dame)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->init_coord_dame_or_rafle = give_coord(g, iw, indMovePawn);

    mem->chainy = rafleNGE(g, indMovePawn, rafleTree, rafle, pos_dame);
    assertAndLog(mem->chainy != NULL && !dis_empty(mem->chainy), "rafle Deter aucune rafle");
    generateCloudDuePawnMove(g, mem);

    return mem;
}

memory_move_t *queenDeplDeter(Game *g, int indMovePawn, Coord pos_dame, moveType type)
{
    // c'est un déplacement simple de la dame sans rafle
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->init_coord_dame_or_rafle = give_coord(g, iw, indMovePawn);
    queenDepl(g, indMovePawn, iw, pos_dame, true);

    generateCloudDuePawnMove(g, mem);

    return mem;
}

memory_move_t *biDeplDeter(Game *g, int indMovePawn, moveType type)
{
    memory_move_t *mem = initMemMove(indMovePawn, type);

    mem->ghost_pawn_created_bidepl = biDeplNGE(g, g->is_white, indMovePawn);
    int indNewGhost = ind_from_coord(g, mem->ghost_pawn_created_bidepl.c);

    // Le pion en avançant peut tuer le nuage
    if (canStormBreaks(g, indMovePawn, g->is_white))
    {
        initTabIssueColor(g, CREATE_CLOUD_TAB, mem, g->is_white);
    }
    else if (canStormBreaks(g, indNewGhost, g->is_white))
    {
        initTabIssueColor(g, CREATE_CLOUD_TAB, mem, g->is_white);
    }
    return mem;
}

void cancelPromotionDeter(Game *g, memory_move_t *mem)
{
    // int ind = ind_from_coord(g, mem->movePawn.coord);
    // cancelPromotion(g, ind, mem->pos_potential_foe_from_prom, mem->is_white);
}

void cancelPawnMoveDeter(Game *g, memory_move_t *mem)
{
    bool iw = mem->is_white;

    int ind = ind_from_coord(g, mem->movePawn.coord);
    pawnMoveCancel(g, iw, ind, mem->left);
}

void cancelPawnMoveBackDeter(Game *g, memory_move_t *mem)
{
    cancelAllMoveBack(g, mem);
}

void cancelBiDeplDeter(Game *g, memory_move_t *mem)
{
    int ind = ind_from_coord(g, mem->movePawn.coord);
    cancelBidepl(g, ind, mem->ghost_pawn_created_bidepl, mem->is_white);
}

void cancelQueenDeplDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->movePawn.coord);
    Coord pos_init = mem->init_coord_dame_or_rafle;
    change_pawn_place(g, ind, mem->is_white, pos_init.i, pos_init.j);
}

void cancelRafleDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->movePawn.coord);
    cancelRafle(g, ind, mem->init_coord_dame_or_rafle, mem->chainy, mem->is_white);
}

void cancelLienAmitieDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->movePawn.coord);
    cancelLienAmitie(g, ind, mem->lig, mem->col, mem->is_white);
}

void cancelLienEnnemitieDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->movePawn.coord);
    cancelLienEnnemitie(g, ind, mem->lig, mem->col, mem->is_white);
}