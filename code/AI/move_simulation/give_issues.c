#include "give_issues.h"

void initTabIssue(Game *g, int what_kind_of_creation, memory_move_t *mem)
{
    bool color = !g->is_white;

    switch (what_kind_of_creation)
    {
    case CREATE_CLOUD_TAB:
        mem->lenghtIssues = g->lengthCloud[color];
        mem->issues = malloc(mem->lenghtIssues * sizeof(issue_t));

        maillon *l = g->cloud[color];
        int i = 0;
        while (!is_empty(l))
        {
            int ind = get(l);
            float pba_inv = get_pawn_value(g, color, ind, PBA);
            mem->issues[i].pba = 1.0 / pba_inv;
            mem->issues[i].pos_survivor = give_coord(g, color, ind);
            l = l->next;
            i++;
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
}

void signCloudDuePawnMove(Game *g, memory_move_t *mem)
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
    if (index != VOID_INDEX)
        stormBreaksNGE(g, !iw, mem->load_cloud_other, mem->survivor, mem->issues[index].pos_survivor);
}

void cancelSelectedIssue(Game *g, memory_move_t *mem)
{
    bool iw = g->is_white;
    int index_origin = mem->lenghtIssues - 1;
    if (index_origin != VOID_INDEX)
        change_pawn_place_newbie(g, mem->survivor->ind, !iw, mem->issues[index_origin].pos_survivor);

    if (!cis_empty(mem->load_cloud_other))
    {
        recreateCloud(g, mem->load_cloud_other, mem->survivor, !iw);
    }
}

memory_move_t *issuePawnMove(Game *g, int indMovePawn, bool left)
{
    // Test pawn move remove, on suppose qu'on peut jouer, et qu'on a deja une fonction pour faire jouer l'ami en arrière

    bool iw = g->is_white;
    pawnMoveNGE(g, iw, indMovePawn, left);
    // Attention l'indice de renvoie du friend est dans le structure g

    memory_move_t *mem = initMemMove(indMovePawn);
    mem->left = left;

    // signCloudDuePawnMove(g, indMovePawn, mem->survivor, mem->load_cloud_other);
    signCloudDuePawnMove(g, mem);

    // endTurnGameManagementSimple(g, indMovePawn);

    return mem;
}

memory_move_t *issuePromotion(Game *g, int indPawn)
{

    memory_move_t *mem = initMemMove(indPawn);
    initTabIssue(g, CREATE_PROM_TAB, mem);
    return mem;
}

void cancelIssuePromotion(Game *g, memory_move_t *mem)
{
    cancelPromotion(g, mem->indMovePawn, mem->ind_potential_foe);
}

memory_move_t *issueMoveBack(Game *g, int indMovePawnBack, bool left)
{
    // On suppose que le move back est faisable
    pawnMoveBackNGE(g, indMovePawnBack, left);
    // la fonction ci dessus remet g->ind_move_back a VOID_INDEX

    memory_move_t *mem = initMemMove(indMovePawnBack);
    mem->left = left;
    // Desecrate
    signCloudDuePawnMove(g, mem);
    return mem;
}

memory_move_t *issueRafle(Game *g, int indMovePawn)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn);
    mem->init_coord = give_coord(g, iw, indMovePawn);
    mem->chainy = rafleNGE(g, indMovePawn);

    signCloudDuePawnMove(g, mem);

    return mem;
}

memory_move_t *issueQueenDepl(Game *g, int indMovePawn, queen_move_t coords)
{
    bool iw = g->is_white;
    memory_move_t *mem = initMemMove(indMovePawn);
    mem->init_coord = give_coord(g, iw, indMovePawn);
    mem->chainy = queenDeplNGE(g, indMovePawn, iw, coords);

    signCloudDuePawnMove(g, mem);
    return mem;
}