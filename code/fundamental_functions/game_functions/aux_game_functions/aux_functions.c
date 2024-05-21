#include "aux_functions.h"

/*







   AUX FUNCTIONS






*/

// On peut améliorer cette fonction
void copy_remove_pawn_from_index_to_index(Game *g, int indStart, int indArrive, bool color)
{
    assert(isValidIndexInGame(g, indStart, color));
    assert(isValidIndex(indArrive));
    if (indStart != indArrive)
    {
        int lig = get_pawn_value(g, color, indStart, LIG);
        int col = get_pawn_value(g, color, indStart, COL);
        g->damier[lig][col].ind_pawn = VOID_INDEX;

        for (int k = 1; k < 9; k++)
        {
            put_pawn_value(g, color, indArrive, k, get_pawn_value(g, color, indStart, k));
        }
        pawn_default_value_new(g, indStart, color);

        // Sur le damier
        g->damier[get_pawn_value(g, color, indArrive, LIG)][get_pawn_value(g, color, indArrive, COL)].ind_pawn = indArrive;
        g->damier[get_pawn_value(g, color, indArrive, LIG)][get_pawn_value(g, color, indArrive, COL)].pawn_color = color;
    }
}

void pawn_default_value_new(Game *g, int ind, bool color)
{
    /* Initialize pawn with default values, identify by its index and color */
    assert(isValidIndex(ind));
    put_pawn_value(g, color, ind, ALIVE, 0);
    put_pawn_value(g, color, ind, COL, -1);
    put_pawn_value(g, color, ind, LIG, -1);
    put_pawn_value(g, color, ind, QUEEN, 0);
    put_pawn_value(g, color, ind, COLOR, int_to_bool(color));
    put_pawn_value(g, color, ind, FRIENDLY, -1);
    put_pawn_value(g, color, ind, ENNEMY, -1);
    put_pawn_value(g, color, ind, PBA, 1);
}

// We are sure about the pawn we delete (no check control so be careful)
// Kills the indicated pawn and applies all the necessary rules
// Les ghosts pawn ne peuvent pas se manger entre eux: attention dans les fonctions eatPawn. Et un pion plein detruit le nuage avant de
// manger un ghost pawn
void killPawn(Game *g, Case **damier, int i, int j)
{
    // printf("i, j = %d, %d\n", i, j);
    // DO NOT remove a pawn from the cloud
    assert(inGame(i, j));
    if (!freeCase(damier[i][j]))
    {
        int indPawn = damier[i][j].ind_pawn;
        bool color = damier[i][j].pawn_color;
        assert(isValidIndexInGame(g, indPawn, color));
        int indEnnemy = get_pawn_value(g, color, indPawn, ENNEMY);
        int indAmi = get_pawn_value(g, color, indPawn, FRIENDLY);
        bool is_queen = int_to_bool(get_pawn_value(g, color, indPawn, QUEEN));
        if (indEnnemy != VOID_INDEX)
        {
            put_pawn_value(g, !color, indEnnemy, ENNEMY, -1);
            put_pawn_value(g, !color, indEnnemy, QUEEN, 1);
            decrBothTab(g->nbFoe);
        }
        else if (indAmi != VOID_INDEX)
        {
            bool opposee_is_queen = int_to_bool(get_pawn_value(g, !color, indAmi, QUEEN));
            put_pawn_value(g, !color, indAmi, FRIENDLY, -1);
            if (is_queen) g->nbQueenWithFriend[color]--;
            else g->nbFriendNoQueen[color]--;

            if (opposee_is_queen) g->nbQueenWithFriend[!color]--;
            else g->nbFriendNoQueen[!color]--;
        }
        else if (is_queen) g->nbQueenWithoutFriend[color]--;
        damier[i][j].ind_pawn = -1;
        /* Supprimer l'indice et optimiser la memoire */
        copy_remove_pawn_from_index_to_index(g, g->nb_pawns[color] - 1, indPawn, color);
        g->nb_pawns[color]--;
    }
    else {
        printv("Personne a tuer ici");
        assert(false);
    }
}

void killPawnByInd(Game *g, bool color, int ind)
{
    assert(isValidIndexInGame(g, ind, color));
    int i = get_pawn_value(g, color, ind, LIG);
    int j = get_pawn_value(g, color, ind, COL);
    assert(inGame(i, j));
    killPawn(g, g->damier, i, j);
}

/*if (!freeCase(damier[i][j]))
    {
        int indPawn = damier[i][j].ind_pawn;
        if (pawns[indPawn].ennemy != -1)
        {
            Npawns[pawns[indPawn].ennemy].ennemy = -1;
            Npawns[pawns[indPawn].ennemy].queen = true;
        }
        if (pawns[indPawn].friendly != -1)
        {
            Npawns[pawns[indPawn].friendly].friendly = -1;
        }
        pawn_default_value_new(pawns, indPawn, pawns[indPawn].color);
        damier[i][j].ind_pawn = -1;
    }*/

/* Put the pawn in a specific case (lig, col).
Useful for queen_move and can be used by us
to go faster for checks : place pawns where we want */

void change_pawn_place(Game *g, int ind, bool color, int lig, int col)
{
    g->damier[get_pawn_value(g, color, ind, LIG)][get_pawn_value(g, color, ind, COL)].ind_pawn = -1;
    put_pawn_value(g, color, ind, LIG, lig);
    put_pawn_value(g, color, ind, COL, col);
    g->damier[lig][col].ind_pawn = ind;
    g->damier[lig][col].pawn_color = color;
}

/* May be useful later */
// Case forbiddenCase()
// {
//     /* Return a case that is a case error. When a function return this case, that mean it doesn't return any case (it doesn't find
//     an allowed case...) */
//     Case c;
//     c.color = false;
//     c.ind_pawn = IND_PB; // That's the important thing
//     c.pawn_color = false;
//     return c;
// }

// Version de changeForEat qui n'imprime pas les changements effectues
// Entree : deux tableaux de pions, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
// et deux entiers add0 et add1 qui indiquent la direction dans laquelle manger
// Sortie : modifie le plateau de maniere a ce que le pion d'indice ind ait mange dans la direction indique
// et retourne l'index du pion mange
int nonLoggingChangeForEat(Game *g, bool color, int indEater, int i, int j, int add0, int add1)
{

    int indVictim = g->damier[i + add0][j + add1].ind_pawn;
    // assert(ind > -1);
    change_pawn_place(g, indEater, color, i + 2 * add0, j + 2 * add1);

    // Kill pawn indVictim
    put_pawn_value(g, !color, indVictim, ALIVE, 0);
    g->damier[i + add0][j + add1].ind_pawn = VOID_INDEX;
    // printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return indVictim;
}

// promotes the indicated pawn, un ghost pawn ne peut pas devenir une dame
void promote(Game *g, bool is_white, int ind)
{
    pawn *p = &(g->allPawns[is_white][ind]);
    p->queen = true;
    if (has_friend(g, ind, is_white)) g->nbQueenWithFriend[is_white]++;
    else g->nbQueenWithoutFriend[is_white]++;
    if (p->ennemy != NEUTRAL_IND)
    {
        pawn *ennemyPawn = &(g->allPawns[!is_white][p->ennemy]);
        killPawn(g, g->damier, ennemyPawn->lig, ennemyPawn->col);
    }
}

void createPawn(Game *g, bool color, int i, int j)
{
    // Rajoute un pion dans la liste des pions de la meme couleur. Attention taille du tableau
    assert(g->nb_pawns[color] < MAX_NB_PAWNS);
    int new_ind = g->nb_pawns[color];

    pawn_default_value_new(g, new_ind, color);
    // // Par securite, remet par defaut les valeurs pour creer le nouveau pion

    put_pawn_value(g, color, new_ind, ALIVE, 1);
    put_pawn_value(g, color, new_ind, LIG, i);
    put_pawn_value(g, color, new_ind, COL, j);

    g->damier[i][j].ind_pawn = new_ind;
    g->damier[i][j].pawn_color = color;

    // Just need to increment, we have enough place (only NB_PAWNS pawns to promote)
    g->nb_pawns[color]++;
}

void simplyPawnMove(Game *g, bool is_white, int ind, bool left)
{
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g, ind, is_white, i + di, j + dj);
}

void putPawnMoveBack(Game *g, bool left)
{
    /* Give the coord of the case where moves back a pawn when it is linked with a friend and the friend has just moved. Return (-1, -1)
    if no case available */
    int ind = g->ind_move_back;
    bool is_white = g->is_white;
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;

    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    g->coordForMoveBack.i = VOID_INDEX;
    g->coordForMoveBack.j = VOID_INDEX;

    if (canMoveBack(g, is_white, ind, left))
    {
        g->coordForMoveBack.i = i - di;
        g->coordForMoveBack.j = j + dj;
    }
    else if (canMoveBack(g, is_white, ind, !left))
    {
        g->coordForMoveBack.i = i - di;
        g->coordForMoveBack.j = j - dj;
    }
}

// Ca bug

// void stormBreaks(Game *g, bool color, int indSurvivor)
// {
//     maillon *l = g->cloud[color];
//     int iSurvivor = get_pawn_value(g, color, indSurvivor, LIG);
//     int jSurvivor = get_pawn_value(g, color, indSurvivor, COL);

//     put_pawn_value(g, color, indSurvivor, PBA, 1);
//     while (!is_empty(l))
//     {
//         int ind = pop(l);
//         if (ind != g->damier[iSurvivor][jSurvivor].ind_pawn)
//             ;
//         {
//             killPawn(g, g->damier, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL));
//         }
//     }
//     g->lengthCloud[color] = 0;

//     // C'est une fonction mutuellement recursive car le pion foudroyer peut etre pres du nuage de la couleur opposee. On verifie donc
//     // dans l'autre nuage

//     if (canStormBreaksForTheOthers(g, g->damier[iSurvivor][jSurvivor].ind_pawn, color))
//         AleatStormBreaks(g, !color);
// }

void AleatStormBreaks(Game *g, bool color)
{
    maillon *l = g->cloud[color];
    int ind = VOID_INDEX;

    while (!is_empty(l))
    {
        ind = pop(l);
        if (is_empty(l))
        {
            put_pawn_value(g, color, ind, PBA, 1);
            break;
        }
        else
        {
            killPawn(g, g->damier, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL));
        }
    }
    g->lengthCloud[color] = 0;

    // C'est une fonction recursive car le pion foudroyer peut etre pres du nuage de la couleur opposee. On verifie donc
    // dans l'autre nuage

    printf("%d %d %d %d", get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL), ind, color);
    flush();
    if (ind != VOID_INDEX && canStormBreaksForTheOthers(g, ind, color))
        AleatStormBreaks(g, !color);
}

void AleatStormBreaksNGE(Game *g, bool color, cloud_chain *load, ind_pba_t *survivor)
{
    // Liste chaine et valeur du pawn survivor modifies par effet de bord
    maillon *l = g->cloud[color];
    int ind = VOID_INDEX;

    while (!is_empty(l))
    {
        ind = pop(l);

        if (is_empty(l))
        {
            survivor->ind = ind;
            survivor->pba = get_pawn_value(g, color, ind, PBA);
            put_pawn_value(g, color, ind, PBA, 1);
            break;
        }
        else
        {
            tcloud k = {.coord = {.i = get_pawn_value(g, color, ind, LIG),
                                  .j = get_pawn_value(g, color, ind, COL)},
                        .pba = get_pawn_value(g, color, ind, PBA)};
            cpush(load, k);
            killPawn(g, g->damier, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL)); // Seg
        }
    }
    g->lengthCloud[color] = 0;
}

void handleCloudDuePawnMoveNGE(Game *g, int indMovedPawn, ind_pba_t *survivor, cloud_chain *l)
{
    bool is_white = g->is_white;
    if (canStormBreaksForTheOthers(g, indMovedPawn, is_white))
    {
        AleatStormBreaksNGE(g, !is_white, l, survivor);
    }
}

queen_move_t CanMoveOrEatQueen(Game *g, bool color, int lig, int col, Case **damier, int ind)
{
    // Check if the move of the queen is possible and move the queen or eat the next pawn in her path
    lig = color ? NB_CASE_LG - lig - 1 : lig;
    int pcol = get_pawn_value(g, color, ind, COL);
    int plig = get_pawn_value(g, color, ind, LIG);
    int dcol = col - pcol;
    int dlig = lig - plig;
    Case c;
    if (abs(dlig) == abs(dcol) && dcol != 0)
    {
        int add_lig = dlig / abs(dlig);
        int add_col = dcol / abs(dcol);
        for (int i = 1; i < abs(dcol) + 1; i++)
        {
            c = damier[plig + add_lig * i][pcol + add_col * i];
            if (!freeCase(c))
            {
                // Here the queen can eat a pawn
                int new_lig = add_lig + plig + add_lig * i;
                int new_col = add_col + pcol + add_col * i;
                // Check if the queen can go here
                if (c.pawn_color == !color && inGame(new_lig, new_col) && freeCase(damier[new_lig][new_col]) &&
                    !isInCloud(g, !color, ind_from_coord(g, new_lig - add_lig, new_col - add_col)))
                {
                    queen_move_t res = {.pos_dame = {.i = new_lig, .j = new_col}, .pos_eaten_pawn = {.i = new_lig - add_lig, .j = new_col - add_col}};
                    return res;
                }
                else if (c.pawn_color == color)
                {
                    // printf("jump sheep");
                    queen_move_t res = {.pos_dame = {.i = VOID_INDEX, .j = VOID_INDEX}, .pos_eaten_pawn = {.i = VOID_INDEX, .j = VOID_INDEX}};
                    return res; // Pb: the queen jumps a pawn of her own color or a ghost pawn
                }
            }
        }
        // Only move the pawn: nothing in her path
        if (inGame(lig, col) && freeCase(damier[lig][col]))
        {
            queen_move_t res = {.pos_dame = {.i = lig, .j = col}, .pos_eaten_pawn = {.i = VOID_INDEX, .j = VOID_INDEX}};
            return res;
        }
    }
    queen_move_t res = {.pos_dame = {.i = VOID_INDEX, .j = VOID_INDEX}, .pos_eaten_pawn = {.i = VOID_INDEX, .j = VOID_INDEX}};
    return res; // No case was found
}

void freeCloud(maillon *l)
{
    while (!is_empty(l))
    {
        pop(l);
    }
    free(l);
}

// Memory Function
void free_game(Game *g)
{
    freeCloud(g->cloud[true]);
    freeCloud(g->cloud[false]);
    if (g->currentTree != emptyTree)
    {
        pathTreeFree(g->currentTree);
    }
    if (g->currentRafle != NULL)
    {
        pathFree(g->currentRafle);
    }
    free(g);
}

Coord give_coord(Game *g, bool iw, int ind)
{
    Coord init_coord = {.i = get_pawn_value(g, iw, ind, LIG),
                        .j = get_pawn_value(g, iw, ind, COL)};
    return init_coord;
}

void doubleTabInit(int t[2]) {
    t[0] = 0;
    t[1] = 0;
}

void incrBothTab(int t[2]) {
    t[0]++;
    t[1]++;
}

void decrBothTab(int t[2]) {
    t[0]--;
    t[1]--;
}