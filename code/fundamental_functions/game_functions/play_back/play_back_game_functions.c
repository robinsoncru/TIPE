#include "play_back_game_functions.h"

void pawnMoveNGE(Game *g, bool is_white, int ind, bool left)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    ;
    pawn p = get_pawn(g, is_white, ind);
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g, ind, is_white, i + di, j + dj);
    if (p.friendly > 0)
    {
        for (int i = 0; i < MAX_NB_PAWNS; i++)
        {
            if (isValidIndexInGame(g, i, !is_white) && getFriendByInd(g, ind, i, is_white) && !alreadyInList(g->inds_move_back, i))
            {
                push(g->inds_move_back, i);
            }
            // le pion indique a partir de son indice
        }
    }
}

void pawnMoveCancel(Game *g, bool is_white, int ind, bool left)
{
    pawn p = get_pawn(g, is_white, ind);
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g, ind, is_white, i - di, j - dj);
}

void recreateCloud(Game *g, cloud_chain *l, int indFromCaseSurvivor, int pbaSurvivor, bool iw)
{

    put_pawn_value(g, iw, indFromCaseSurvivor, PBA, pbaSurvivor);
    push(g->cloud[iw], indFromCaseSurvivor); // Pion dernier sortie remie en premier (file)

    while (!cis_empty(l))
    {
        tcloud data = cpop(l);
        int i = data.coord.i;
        int j = data.coord.j;
        createPawn(g, iw, i, j);
        int ind = get_case_damier(g, i, j).ind_pawn;
        put_pawn_value(g, iw, ind, PBA, data.pba);
        push(g->cloud[iw], ind);
        g->lengthCloud[iw]++;
    }

    g->lengthCloud[iw]++;
}

Coord promotionNGE(Game *g, bool iw, int ind, int index_choice)
{

    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    // Return the index of the ennemy pawn created, -1 else
    int choice;
    if (index_choice == VOID_INDEX)
    {
        // choice = rand() % 3;
        choice = 2;
        printf("(choix %d)", choice);
    }
    else
    {
        choice = index_choice;
    }
    Coord pos_final = {.i = -1, .j = IND_NOTHING_HAPPENED};
    if (choice == 1)
    {
        promote(g, iw, ind);
        pos_final.j = IND_GLORY_QUEEN;
        return pos_final;
    }
    else if (choice == 2)
    {
        int i = get_pawn_value(g, iw, ind, LIG);
        int j = get_pawn_value(g, iw, ind, COL);

        // printf("Quantic %d %d", i, j);

        // Kill the former pawn
        killPawn(g, i, j);

        // Give birth to the ennemy pawn
        createPawn(g, !iw, i, j);
        // On verifie s'il eclate un nuage ou devient reine dans les GameManagements

        pos_final.i = i;
        pos_final.j = j;
        return pos_final;
    }

    return pos_final;
}

void cancelPromotion(Game *g, int indPawnBeforeProm, Coord pos_potential_foe_from_prom)
{
    bool iw = g->is_white;
    int i = pos_potential_foe_from_prom.i;
    int j = pos_potential_foe_from_prom.j;
    if (inGame(i, j))
    {
        killPawn(g, i, j);
        createPawn(g, iw, i, j);
    }

    else
        put_pawn_value(g, iw, indPawnBeforeProm, QUEEN, 0);
}

ind_bool_t biDeplNGE(Game *g, bool color, int ind)
{
    // On suppose le coup legal
    bool depl = int_to_bool(rand() % 2);
    // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
    int dj = depl ? 1 : -1;

    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    int di = color ? 1 : -1;
    // Creer un pion a droite ou a gauche aleatoirement
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + dj;
    createPawn(g, color, newLig, newCol);
    int newInd = get_case_damier(g, newLig, newCol).ind_pawn;
    put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

    // Rajoute dans le cloud
    if (!isInCloud(g, color, ind))
    {
        push(g->cloud[color], ind);
        g->lengthCloud[color]++;
    }
    push(g->cloud[color], newInd);

    g->lengthCloud[color]++;

    // Deplace le pion de l'autre cote
    put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
    simplyPawnMove(g, color, ind, depl);
    ind_bool_t data = {.b = depl, .ind = newInd};
    return data;
}

void cancelBidepl(Game *g, int oldInd, ind_bool_t data)
{
    bool iw = g->is_white;
    bool left = data.b;
    int newInd = data.ind;
    pop(g->cloud[iw]); // Supress the last pawn pushed in the cloud, which is indNew
    g->lengthCloud[iw]--;
    killPawnByInd(g, iw, newInd);

    // Move back
    pawnMoveCancel(g, iw, oldInd, left);

    int pba = get_pawn_value(g, iw, oldInd, PBA);
    put_pawn_value(g, iw, oldInd, PBA, pba / 2);
    if (!isInCloud(g, iw, oldInd))
    {
        // If the pawn exit the cloud
        pop(g->cloud[iw]);
        g->lengthCloud[iw]--;
    }
}

data_chain *eatRafleNGE(Game *g, int indEater, bool is_white, PathTree *t, Path *r)
{
    PathTree *workTree = t;
    uint8_t dirCode;
    int di, dj;

    data_chain *chainy = dcreate_list();

    while (pathTreeDepth(workTree) > 0)
    {
        switch (pathTreeNBChilds(workTree))
        {
        case 1:
            dirCode = pathTreeFirstChild(workTree);
            break;
        default:
            dirCode = pathPopFirstIn(r);
            break;
        }
        getDirsFromCode(dirCode, &di, &dj);

        int lig_pawn_eaten = get_pawn_value(g, is_white, indEater, LIG) + di;
        int col_pawn_eaten = get_pawn_value(g, is_white, indEater, COL) + dj;
        int eatenInd = get_case_damier(g, lig_pawn_eaten, col_pawn_eaten).ind_pawn;

        coord_tab_t *tamis = friendTabToCoordTab(g, eatenInd, !is_white);

        int indEnn = get_pawn_value(g, !is_white, eatenInd, ENNEMY);

        pawn_info data_eaten = {.relationship = {.friendsId = tamis,
                                                 .pos_foe = coord_from_ind(g, !is_white, indEnn),
                                                 .queen = int_to_bool(get_pawn_value(g, !is_white, eatenInd, QUEEN))},
                                .coord = {.i = lig_pawn_eaten, .j = col_pawn_eaten}};

        dpush(chainy, data_eaten);
        pawnEat(g, indEater, is_white, di, dj);
        workTree = pathTreeChild(workTree, dj, di);
    }

    return chainy;
}

data_chain *rafleNGE(Game *g, int indMovePawn)
{
    assert(g->currentTree == emptyTree);
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    bool isWhite = g->is_white;
    assert(isValidIndexInGame(g, indMovePawn, isWhite));

    g->currentTree = rafleTreeCalc(g, isWhite, g->ind_move);

    printf("lazyRafle called\n");
    Path *r = lazyRafle(g->currentTree);
    printf("eatRafle called\n");
    data_chain *chainy = eatRafleNGE(g, g->ind_move, g->is_white, g->currentTree, r);
    printf("pathFree called\n");
    pathFree(r);
    return chainy;
}

void cancelRafle(Game *g, int indMovedPawn, Coord init_pos, data_chain *chainy)
{
    bool iw = g->is_white;
    while (!dis_empty(chainy))
    {
        pawn_info reborn_pawn = dpop(chainy);
        int i = reborn_pawn.coord.i;
        int j = reborn_pawn.coord.j;
        createPawn(g, !iw, i, j);
        int reborn_ind = get_case_damier(g, i, j).ind_pawn;
        coord_tab_t *t = reborn_pawn.relationship.friendsId;
        Coord c;
        while (!ctis_empty(t))
        {
            c = ctpop(t);
            putFriendByInd(g, reborn_ind, ind_from_coord(g, c), iw, true);
        }
        ctfree(reborn_pawn.relationship.friendsId);
        reborn_pawn.relationship.friendsId = NULL;
        c = reborn_pawn.relationship.pos_foe;
        put_pawn_value(g, !iw, reborn_ind, ENNEMY, ind_from_coord(g, c));
        put_pawn_value(g, !iw, reborn_ind, QUEEN, bool_to_int(reborn_pawn.relationship.queen));
    }
    free(chainy);

    change_pawn_place(g, indMovedPawn, iw, init_pos.i, init_pos.j);
}

void cancelDeplQueen(Game *g, int ind_queen, data_chain *chainy, Coord init_coord)
{
    cancelRafle(g, ind_queen, init_coord, chainy);
}

void lienAmitieNGE(int lig, int col, int ind, bool is_white, Game *g)
{
    assertAndLog(is_empty(g->inds_move_back), "LienAmitiePmetreNGE : Il reste des amis à déplacer");

    // !get_pawn_value(g, !iw, c.ind_pawn, QUEEN) && get_pawn_value(g, !iw, c.ind_pawn, ENNEMY) == VOID_INDEX

    Case c = get_case_damier(g, lig, col);
    assertAndLog((c.pawn_color == !is_white && isValidIndexInGame(g, c.pawn_color, !is_white)),
                 "LienAmitieNGE : pb couleur ou index pas valide");

    assertAndLog(!getFriendByInd(g, ind, c.ind_pawn, is_white), "lienAmitieNGE: ils sont deja amis");
    putFriendByInd(g, ind, c.ind_pawn, is_white, true);

    g->nbFriendNoQueen[is_white]++;
    g->nbFriendNoQueen[!is_white]++;
}

void cancelLienAmitie(Game *g, int indPawn, int lig, int col)
{
    bool iw = g->is_white;
    int amiInd = get_case_damier(g, lig, col).ind_pawn;
    assert(amiInd != VOID_INDEX);
    putFriendByInd(g, indPawn, amiInd, iw, false);
}

void lienEnnemitieNGE(bool is_white, int lig, int col, int ind, Game *g)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");

    Case c = get_case_damier(g, lig, col);
    assert(c.ind_pawn != -1);
    put_pawn_value(g, is_white, ind, 2, c.ind_pawn);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 2, ind);
    incrBothTab(g->nbFoe);
}

void cancelLienEnnemitie(Game *g, int indPawn, int lig, int col)
{
    bool iw = g->is_white;
    int ennInd = get_case_damier(g, lig, col).ind_pawn;
    assert(ennInd != VOID_INDEX);
    put_pawn_value(g, iw, indPawn, ENNEMY, -1);
    put_pawn_value(g, !iw, ennInd, ENNEMY, -1);
}
