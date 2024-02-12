#include "AI_functions.h"

void pawnMoveAI(Game *g, int indMovePawn, bool left)
{
    // Test pawn move remove, on suppose qu'on peut jouer, et qu'on a deja une fonction pour faire jouer l'ami en arrière

    bool iw = g->is_white;
    pawnMoveNGE(g, iw, indMovePawn, left);
    // Attention l'indice de renvoie du friend est dans le structure g

    cloud_chain *load_cloud_other = ccreate_list();
    ind_pba_t *survivor = malloc(sizeof(ind_pba_t));
    // Desecrate
    handleCloudDuePawnMoveNGE(g, indMovePawn, survivor, load_cloud_other);
    /* Desecrate the endTurnGameManagement to pawnMove to
   check the storm */

    endTurnGameManagementSimple(g, indMovePawn);

    picture_this(g);

    // Here recreate the cloud
    if (!cis_empty(load_cloud_other))
    {
        recreateCloud(g, load_cloud_other, survivor, !iw);
    }

    pawnMoveCancel(g, iw, indMovePawn, left);
    free(survivor);
    free(load_cloud_other);
}

void promotionIA(Game *g, int indMovePawn)
{

    int ind_potential_foe = promotionNGE(g, indMovePawn);

    endTurnGameManagementSimple(g, indMovePawn);

    picture_this(g);
    cancelPromotion(g, indMovePawn, ind_potential_foe);
}

void pawnMoveBackAI(Game *g, int indMovePawnBack, bool left)
{
    // On suppose que le move back est faisable
    bool iw = g->is_white;
    pawnMoveBackNGE(g, indMovePawnBack, left);
    // la fonction ci dessus remet g->ind_move_back a VOID_INDEX
    
    cloud_chain *load_cloud_other = ccreate_list();
    ind_pba_t *survivor = malloc(sizeof(ind_pba_t));
    // Desecrate
    handleCloudDuePawnMoveNGE(g, indMovePawnBack, survivor, load_cloud_other);

    endTurnGameManagementSimple(g, indMovePawnBack);

    picture_this(g);

    if (!cis_empty(load_cloud_other))
    {
        recreateCloud(g, load_cloud_other, survivor, !iw);
    }

    cancelMoveBack(g, indMovePawnBack, left);
    
    free(load_cloud_other);
    free(survivor);
}

void biDeplAI(Game *g, int indMovePawn)
{

    ind_bool_t data = biDeplNGE(g, g->is_white, indMovePawn);

    // Desecrate

    endTurnGameManagementSimple(g, indMovePawn);

    picture_this(g);
    cancelBidepl(g, indMovePawn, data);
}

void queenDeplAI(Game *g, int indMovePawn, queen_move_t coords)
{
    bool iw = g->is_white;
    Coord init_coord = give_coord(g, iw, indMovePawn);
    data_chain *chainy = queenDeplNGE(g, indMovePawn, iw, coords);
    picture_this(g);

    endTurnGameManagementSimple(g, indMovePawn);

    cancelDeplQueen(g, indMovePawn, chainy, init_coord);
}

void rafleAI(Game *g, int indMovePawn)
{
    bool iw = g->is_white;
    Coord init_coord = give_coord(g, iw, indMovePawn);
    data_chain *chainy = rafleNGE(g, indMovePawn);
    picture_this(g);
    cancelRafle(g, indMovePawn, init_coord, chainy);
}

void lienAmitieAI(Game *g, int indPawn, int lig, int col) {
    if (g->is_white) lig = NB_CASE_LG-lig-1;
    lienAmitieNGE(lig, col, g, indPawn);
    endTurnGameManagementSimple(g, indPawn);
    picture_this(g);
    cancelLienAmitie(g, indPawn, lig, col);
}

void lienEnnemitieAI(Game *g, int indPawn, int lig, int col) {
    if (g->is_white) lig = NB_CASE_LG-lig-1;
    lienEnnemitieNGE(lig, col, g, indPawn);
    endTurnGameManagementSimple(g, indPawn);
    picture_this(g);
    cancelLienEnnemitie(g, indPawn, lig, col);
}