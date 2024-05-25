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
    if (p.friendly)
    {
        for (int i = 0; i < MAX_NB_PAWNS; i++)
        {
            if (isValidIndexInGame(g, i, !is_white) && getFriendByInd(g, ind, i, is_white))
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

void recreateCloud(Game *g, cloud_chain *l, ind_pba_t *survivor, bool iw)
{

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

    put_pawn_value(g, iw, survivor->ind, PBA, survivor->pba);
    push(g->cloud[iw], survivor->ind);
    g->lengthCloud[iw]++;
}

int promotionNGE(Game *g, int ind)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    ;
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    // Return the index of the ennemy pawn created, -1 else
    bool iw = g->is_white;
    int choice = rand() % 3;
    if (choice == 1)
    {
        put_pawn_value(g, iw, ind, QUEEN, 1);
        printv("QUEEN");
        return VOID_INDEX;
    }
    else if (choice == 2)
    {
        printv("BLACK");
        int i = get_pawn_value(g, iw, ind, LIG);
        int j = get_pawn_value(g, iw, ind, COL);

        // Kill the former pawn
        killPawn(g, i, j);

        // Give birth to the ennemy pawn
        createPawn(g, !iw, i, j);
        int indNew = get_case_damier(g, i, j).ind_pawn;
        if (canBePromoted(g, !iw, indNew))
            promote(g, !iw, indNew);

        return indNew;
    }
    printv("NOTHING");

    return VOID_INDEX;
}

void cancelPromotion(Game *g, int ind_old_friend, int ind_new_foe)
{
    bool iw = g->is_white;
    if (ind_new_foe != -1)
    {
        int i = get_pawn_value(g, iw, ind_new_foe, LIG);
        int j = get_pawn_value(g, iw, ind_new_foe, COL);
        killPawn(g, i, j);
        createPawn(g, iw, i, j);
    }
    else
        put_pawn_value(g, iw, ind_old_friend, QUEEN, 0);
}

ind_bool_t biDeplNGE(Game *g, bool color, int ind)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    ;
    // On suppose le coup legal
    bool depl = int_to_bool(rand() % 2);
    // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
    int dj = depl ? 1 : -1;
    int di = color ? 1 : -1;
    // Creer un pion a droite ou a gauche aleatoirement
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + dj;
    createPawn(g, color, newLig, newCol);
    int newInd = get_case_damier(g, newLig, newCol).ind_pawn;
    put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

    // Rajoute dans le cloud
    if (!isInCloud(g, color, ind))
        push(g->cloud[color], ind);
    push(g->cloud[color], newInd);

    g->lengthCloud[color]++;

    // Deplace le pion de l'autre cote
    put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
    simplyPawnMove(g, color, ind, depl);
    ind_bool_t data = {.ind = newInd, .b = depl};
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

        int *lamis = friendTabToListChaine(g, eatenInd, !is_white);

        pawn_info data_eaten = {.relationship = {.friendsId = lamis,
                                                 .foe = get_pawn_value(g, !is_white, eatenInd, ENNEMY),
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
    ;
    bool isWhite = g->is_white;
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
        int *l = reborn_pawn.relationship.friendsId;
        while (!is_empty(l))
        {
            putFriendByInd(g, reborn_ind, pop(l), iw, true);
        }
        freeIntChain(reborn_pawn.relationship.friendsId);
        reborn_pawn.relationship.friendsId = NULL;
        put_pawn_value(g, !iw, reborn_ind, ENNEMY, reborn_pawn.relationship.foe);
        put_pawn_value(g, !iw, reborn_ind, QUEEN, bool_to_int(reborn_pawn.relationship.queen));
    }
    free(chainy);

    change_pawn_place(g, indMovedPawn, iw, init_pos.i, init_pos.j);
}

data_chain *queenDeplNGE(Game *g, int ind, bool color, queen_move_t tuple_coord)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    ;
    // Suppose que l'entree est valide
    // On a deja la position d'arrivee et le potentiel pion a manger en entree

    // The queen can move on diagonals or eat pawns only by putting herself in front of the first pawn she
    // will eat, eatRafle will do the rest of the job
    // So pos_eaten_pawn is useless here
    int lig = tuple_coord.pos_dame.i;
    int col = tuple_coord.pos_dame.j;
    change_pawn_place(g, ind, color, lig, col);

    // Gonna check if the queen can take a rafle
    data_chain *chainy = rafleNGE(g, ind);

    if (dis_empty(chainy))
    {
        prout;
        push(g->inds_move_back, i);

        // le pion indique a partir de son indice
    }

    return chainy;
}

void cancelDeplQueen(Game *g, int ind_queen, data_chain *chainy, Coord init_coord)
{
    cancelRafle(g, ind_queen, init_coord, chainy);
}

void lienAmitiePmetreNGE(int lig, int col, int ind, bool is_white, Game *g)
{
    assertAndLog(is_empty(g->inds_move_back), "LienAmitiePmetreNGE : Il reste des amis à déplacer");
    
    Case c = get_case_damier(g, lig, col);
    assertAndLog((isValidIndexInGame(g, c.pawn_color, !is_white) && c.pawn_color == !is_white), 
    "LienAmitieNGE : pb couleur ou index pas valide");
    putFriendByInd(g, ind, )
    if (int_to_bool(get_pawn_value(g, is_white, ind, QUEEN)))
    {
        g->nbQueenWithFriend[is_white]++;
        g->nbQueenWithoutFriend[is_white]--;
    }
    else
        g->nbFriendNoQueen[is_white]++;

    if (int_to_bool(get_pawn_value(g, !is_white, c.ind_pawn, QUEEN)))
    {
        g->nbQueenWithFriend[!is_white]++;
        g->nbQueenWithoutFriend[!is_white]--;
    }
    else
        g->nbFriendNoQueen[!is_white]++;
}

void lienAmitieNGE(int lig, int col, Game *g, int indPawn)
{
    // On suppose le coup legal
    bool iw = g->is_white;
    lienAmitiePmetreNGE(lig, col, indPawn, iw, g);
}

void cancelLienAmitie(Game *g, int indPawn, int lig, int col)
{
    bool iw = g->is_white;
    int ennInd = get_case_damier(g, lig, col).ind_pawn;
    assert(ennInd != VOID_INDEX);
    put_pawn_value(g, iw, indPawn, FRIENDLY, -1);
    put_pawn_value(g, !iw, ennInd, FRIENDLY, -1);
}

void lienEnnemitiePmetreNGE(bool is_white, int lig, int col, int ind, Game *g)
{
    assertAndLog(is_empty(g->inds_move_back), "Il reste des amis dans les NGE");
    ;
    Case c = get_case_damier(g, lig, col);
    assert(c.ind_pawn != -1);
    put_pawn_value(g, is_white, ind, 2, c.ind_pawn);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 2, ind);
    incrBothTab(g->nbFoe);
}

void lienEnnemitieNGE(int lig, int col, Game *g, int indPawn)
{
    // Suppose legal move
    bool iw = g->is_white;
    lienEnnemitiePmetreNGE(iw, lig, col, indPawn, g);
}

void cancelLienEnnemitie(Game *g, int indPawn, int lig, int col)
{
    bool iw = g->is_white;
    int ennInd = get_case_damier(g, lig, col).ind_pawn;
    assert(ennInd != VOID_INDEX);
    put_pawn_value(g, iw, indPawn, ENNEMY, -1);
    put_pawn_value(g, !iw, ennInd, ENNEMY, -1);
}
