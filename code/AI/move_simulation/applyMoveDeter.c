#include "applyMoveDeter.h"

void initTabIssueColor(Game *g, int what_kind_of_creation, memory_move_t *mem, bool color)
{
    // Appeler pour les fonctions non déterministe
    // La couleur appelée est celle de l'adversaire
    int_chain *l = NULL;
    int taille_l;
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
        Coord c_init = {.i = -1, .j = -1};
        for (int i = 0; i < 3; i++)
        {
            mem->issues[i].pba_promotion = 3;
            mem->issues[i].pba = -1;
            mem->issues[i].pos_survivor = c_init;
        }

        break;

    case CREATE_CLOUD_PROM_TAB:
        assertAndLog(mem->lenghtIssues == 3, "initTabIssue : Il n'y pas de promotion");

        l = g->cloud[color];
        taille_l = taille_list(l);
        mem->lenghtIssues += (taille_l - 1);
        // On joue directement une config du nuage lors après l'initialisation
        mem->issues = malloc(mem->lenghtIssues * sizeof(issue_t));
        // Les trois premières cases du tableau sont laissées vides
        for (int i = 2; i < taille_l - 1; i++)
        {
            int ind = get(l, i);
            int pba_inv = get_pawn_value(g, color, ind, PBA);
            mem->issues[i].pba = pba_inv;
            mem->issues[i].pos_survivor = give_coord(g, color, ind);
            mem->issues[i].pba_promotion = 3;
        }
        mem->prom_need_break_cloud = true;
        break;
    }
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

void cancelPromotionDeter(Game *g, memory_move_t *mem)
{
    cancelPromotion(g, mem->indMovePawn, mem->pos_potential_foe_from_prom);

    freeMemMove(mem);
}

memory_move_t *moveBackDeter(Game *g, moveType type)
{

    memory_move_t *mem = initMemMove(VOID_INDEX, type);
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

    emptyIntChain(g->inds_move_back); // On supprime les amis g->inds_move_back qui est enregistré

    return mem;
}

memory_move_t *rafleDeter(Game *g, int indMovePawn, PathTree *rafleTree, Path *rafle, moveType type)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->init_coord = give_coord(g, iw, indMovePawn);
    mem->chainy = rafleNGE(g, indMovePawn);

    generateCloudDuePawnMove(g, mem);

    return mem;
}

memory_move_t *queenDeplDeter(Game *g, int indMovePawn, Coord pos_dame, PathTree *rafleTree, Path *rafle, moveType type)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->init_coord = give_coord(g, iw, indMovePawn);
    mem->chainy = queenDepl(g, indMovePawn, iw, pos_dame, true);

    if (dis_empty(mem->chainy))
    {
        generateCloudDuePawnMove(g, mem);
    }
    return mem;
}

void cancelPawnMoveDeter(Game *g, memory_move_t *mem)
{
    bool iw = g->is_white;

    pawnMoveCancel(g, iw, mem->indMovePawn, mem->left);
    freeMemMove(mem);
}

void cancelPawnMoveBackDeter(Game *g, memory_move_t *mem)
{
    cancelAllMoveBack(g, mem);
    freeMemMove(mem);
}

void cancelBiDeplDeter(Game *g, memory_move_t *mem)
{
    cancelBidepl(g, mem->indMovePawn, mem->full_pawn_data);
    freeMemMove(mem);
}

void cancelQueenDeplDeter(Game *g, memory_move_t *mem)
{

    cancelDeplQueen(g, mem->indMovePawn, mem->chainy, mem->init_coord);
    freeMemMove(mem);
}

void cancelRafleDeter(Game *g, memory_move_t *mem)
{

    cancelRafle(g, mem->indMovePawn, mem->init_coord, mem->chainy);
    freeMemMove(mem);
}

void cancelLienAmitieDeter(Game *g, memory_move_t *mem)
{

    cancelLienAmitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}

void cancelLienEnnemitieDeter(Game *g, memory_move_t *mem)
{

    cancelLienEnnemitie(g, mem->indMovePawn, mem->lig, mem->col);
    freeMemMove(mem);
}