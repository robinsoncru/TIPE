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

void lightnightStrike(Game *g, memory_move_t *mem, int index)
{
    // Eclate le nuage si necessaire, index décide de la position du survivant
    bool iw = g->is_white;
    lightnightStrikeColor(g, mem, index, !iw);
}

void lightnightStrikeColor(Game *g, memory_move_t *mem, int index, bool color)
{
    // Eclate le nuage si necessaire, index décide de la position du survivant
    if (!mem->is_deter)
    {
        stormBreaksNGE(g, color, index, mem);
    }
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

    // Le pion en avançant peut tuer le nuage
    if (canStormBreaks(g, indMovePawn, g->is_white))
    {
        initTabIssueColor(g, CREATE_CLOUD_TAB, mem, g->is_white);
    }
    return mem;
}

void cancelPromotionDeter(Game *g, memory_move_t *mem)
{
    int ind = ind_from_coord(g, mem->coordMovePawn);
    cancelPromotion(g, ind, mem->pos_potential_foe_from_prom, mem->is_white);

    freeMemMove(mem);
}

void cancelPawnMoveDeter(Game *g, memory_move_t *mem)
{
    bool iw = mem->is_white;

    int ind = ind_from_coord(g, mem->coordMovePawn);
    pawnMoveCancel(g, iw, ind, mem->left);
    freeMemMove(mem);
}

void cancelPawnMoveBackDeter(Game *g, memory_move_t *mem)
{
    cancelAllMoveBack(g, mem);
    freeMemMove(mem);
}

void cancelBiDeplDeter(Game *g, memory_move_t *mem)
{
    int ind = ind_from_coord(g, mem->coordMovePawn);
    cancelBidepl(g, ind, mem->ghost_pawn_created_bidepl, mem->is_white);
    freeMemMove(mem);
}

void cancelQueenDeplDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->coordMovePawn);
    Coord pos_init = mem->init_coord_dame_or_rafle;
    change_pawn_place(g, ind, mem->is_white, pos_init.i, pos_init.j);
    freeMemMove(mem);
}

void cancelRafleDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->coordMovePawn);
    cancelRafle(g, ind, mem->init_coord_dame_or_rafle, mem->chainy, mem->is_white);
    freeMemMove(mem);
}

void cancelLienAmitieDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->coordMovePawn);
    cancelLienAmitie(g, ind, mem->lig, mem->col, mem->is_white);
    freeMemMove(mem);
}

void cancelLienEnnemitieDeter(Game *g, memory_move_t *mem)
{

    int ind = ind_from_coord(g, mem->coordMovePawn);
    cancelLienEnnemitie(g, ind, mem->lig, mem->col, mem->is_white);
    freeMemMove(mem);
}