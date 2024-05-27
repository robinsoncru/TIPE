#include "applyMoveDeter.h"

void initTabIssue(Game *g, int what_kind_of_creation, memory_move_t *mem)
{
    // Appeler pour les fonctions non déterministe
    bool color = !g->is_white;

    switch (what_kind_of_creation)
    {
    case CREATE_CLOUD_TAB:
        mem->lenghtIssues = g->lengthCloud[color];
        mem->issues = malloc(mem->lenghtIssues * sizeof(issue_t));

        int_chain *l = g->cloud[color];
        for (int i = 0; i < taille_list(l); i++)
        {
            int ind = get(l, i);
            float pba_inv = get_pawn_value(g, color, ind, PBA);
            mem->issues[i].pba = 1.0 / pba_inv;
            mem->issues[i].pos_survivor = give_coord(g, color, ind);
        }
        break;

    case CREATE_PROM_TAB:

        mem->lenghtIssues = 3;
        mem->issues = malloc(3 * sizeof(issue_t));
        for (int i = 0; i < 3; i++)
        {
            mem->issues[i].index = i + 2;
            mem->issues[i].pba = 1.0 / 3.0;
        }

        break;
    }
    mem->is_deter = false;
}

void generateCloudDuePawnMove(Game *g, memory_move_t *mem)
{
    bool is_white = g->is_white;
    if (canStormBreaksForTheOthers(g, mem->indMovePawn, is_white))
    {
        // AleatStormBreaksNGE(g, !is_white, l, survivor);
        initTabIssue(g, CREATE_CLOUD_TAB, mem);
    }
}

void lightnightStrike(Game *g, memory_move_t *mem, int index)
{
    bool iw = g->is_white;
    if (!mem->is_deter)
    {
        assertAndLog(false, "lighnight");
        stormBreaksNGE(g, !iw, mem->load_cloud_other, mem->survivor, mem->issues[index].pos_survivor);
    }
}

void cancelSelectedIssue(Game *g, memory_move_t *mem)
{
    bool iw = g->is_white;
    int index_origin = mem->lenghtIssues - 1;
    if ((index_origin - 1) != VOID_INDEX)
        change_pawn_place_coord(g, mem->survivor->ind, !iw, mem->issues[index_origin].pos_survivor);

    if (!cis_empty(mem->load_cloud_other))
    {
        recreateCloud(g, mem->load_cloud_other, mem->survivor, !iw);
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

    // generateCloudDuePawnMove(g, indMovePawn, mem->survivor, mem->load_cloud_other);
    generateCloudDuePawnMove(g, mem);

    // endTurnGameManagementSimple(g, indMovePawn);

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
    cancelPromotion(g, mem->indMovePawn, mem->ind_potential_foe);

    freeMemMove(mem);
}

memory_move_t *moveBackDeter(Game *g, moveType type)
{
    // On suppose que le move back est faisable
    moveBackNGE(g, true, false, zero_fun);
    // la fonction ci dessus remet g->ind_move_back a VOID_INDEX

    memory_move_t *mem = initMemMove(VOID_INDEX, type);
    // Desecrate
    for (int i = 0; i < taille_list(g->inds_move_back); i++)
    {
        mem->indMovePawn = pop(g->inds_move_back);
        generateCloudDuePawnMove(g, mem);
        if (mem->lenghtIssues > 0) {
            break; // Le nuage implose, il n'existe plus
        }
    }
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

memory_move_t *queenDeplDeter(Game *g, int indMovePawn, queen_move_t coords, PathTree *rafleTree, Path *rafle, moveType type)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn, type);
    mem->init_coord = give_coord(g, iw, indMovePawn);
    mem->chainy = queenDeplNGE(g, indMovePawn, iw, coords);

    generateCloudDuePawnMove(g, mem);
    return mem;
}