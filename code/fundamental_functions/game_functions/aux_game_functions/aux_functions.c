#include "aux_functions.h"

/*







   AUX FUNCTIONS






*/

void changeFriendByInd(Game *g, int indActuStart, int indActuArrive, int indFriend, bool colorActu)
{
    assertAndLog(indActuStart != indActuArrive, "changeFriendByInd : ind de départ = arrivé");
    putFriendByInd(g, indActuStart, indFriend, colorActu, false);
    putFriendByInd(g, indActuArrive, indFriend, colorActu, true);
}

// int *getTabFriendByInd(Game *g, int ind, bool color) {
//     assertAndLog(isValidIndexInGame(g, ind, color), "getTabFriendByInd ind pas valide");
//     if (color) {

//     }
// }

void copy_remove_pawn_from_index_to_index(Game *g, int indStart, int indArrive, bool color)
{

    // On écrase les valeurs de l'indice d'arrivé par les valeurs de l'indice de départ
    assert(isValidIndexInGame(g, indStart, color));
    assert(isValidIndex(indArrive));

    // Il faut supprimer les dépendances des amis et du nuage de l'indice d'arrivé

    // On tue les liens d'amitie de l'indice d'arrivé
    if (get_pawn_value(g, color, indArrive, FRIENDLY) > 0)
    {
        for (int i = 0; i < MAX_NB_PAWNS; i++)
        {
            if (getFriendByInd(g, indArrive, i, color))
            {
                putFriendByInd(g, indArrive, i, color, false);
            }
        }
    }

    if (indStart != indArrive)
    {
        int lig = get_pawn_value(g, color, indStart, LIG);
        int col = get_pawn_value(g, color, indStart, COL);
        put_case_damier(g, lig, col, IND_PAWN_ON_CASE, VOID_INDEX);

        // Il faut gérer les liens amicaux
        pawn p = get_pawn(g, color, indStart);
        if (p.friendly > 0)
        {
            for (int indFriend = 0; indFriend < MAX_NB_PAWNS; indFriend++)
            {

                if (isValidIndexInGame(g, indFriend, !color) && getFriendByInd(g, indStart, indFriend, color))
                {
                    changeFriendByInd(g, indStart, indArrive, indFriend, color);
                }
            }
        }
        if (isValidIndexInGame(g, p.ennemy, !color))
        {
            put_pawn_value(g, !color, p.ennemy, ENNEMY, indArrive);
        }

        for (int k = 1; k < 9; k++)
        {
            if (k != FRIENDLY)
            {
                put_pawn_value(g, color, indArrive, k, get_pawn_value(g, color, indStart, k));
            }
        }
        if (isInCloud(g, color, indStart))
        {
            replaceValueInList(g->cloud[color], indStart, indArrive);
        }

        // Sur le damier
        put_case_damier(g, get_pawn_value(g, color, indArrive, LIG), get_pawn_value(g, color, indArrive, COL), IND_PAWN_ON_CASE, indArrive);
        put_case_damier(g, get_pawn_value(g, color, indArrive, LIG), get_pawn_value(g, color, indArrive, COL), PAWN_COLOR, bool_to_int(color));
    }
    pawn_default_value(g, indStart, color);
}

void pawn_default_value(Game *g, int ind, bool color)
{
    /* Initialize pawn with default values, identify by its index and color */
    assert(isValidIndex(ind));
    put_pawn_value(g, color, ind, ALIVE, 0);
    put_pawn_value(g, color, ind, COL, -1);
    put_pawn_value(g, color, ind, LIG, -1);
    put_pawn_value(g, color, ind, QUEEN, 0);
    put_pawn_value(g, color, ind, COLOR, int_to_bool(color));
    put_pawn_value(g, color, ind, FRIENDLY, 0);
    put_pawn_value(g, color, ind, ENNEMY, -1);
    put_pawn_value(g, color, ind, PBA, 1);
}

// We are sure about the pawn we delete (no check control so be careful)
// Kills the indicated pawn and applies all the necessary rules
// Les ghosts pawn ne peuvent pas se manger entre eux: attention dans les fonctions eatPawn. Et un pion plein detruit le nuage avant de
// manger un ghost pawn
void killPawn(Game *g, int i, int j)
{
    // printf("i, j = %d, %d\n", i, j);
    // DO NOT remove a pawn from the cloud
    assertAndLog(inGame(i, j), "kill pawn pion hors jeu");

    Case c = get_case_damier(g, i, j);
    if (!freeCase(c))
    {
        int indPawn = c.ind_pawn;
        bool color = c.pawn_color;
        assert(isValidIndexInGame(g, indPawn, color));
        int indEnnemy = get_pawn_value(g, color, indPawn, ENNEMY);
        bool is_queen = int_to_bool(get_pawn_value(g, color, indPawn, QUEEN));
        if (indEnnemy != VOID_INDEX)
        {
            put_pawn_value(g, !color, indEnnemy, ENNEMY, -1);
            put_pawn_value(g, !color, indEnnemy, QUEEN, 1);
            decrBothTab(g->nbFoe);
        }
        else
        {
            if (is_queen)
            {
                g->nbQueenWithoutFriend[color]--;
            }
            if (has_friend(g, indPawn, color))
            {
                for (int indAmi = 0; indAmi < MAX_NB_PAWNS; indAmi++)
                {
                    if (isValidIndexInGame(g, indAmi, !color) && getFriendByInd(g, indPawn, indAmi, color))
                    {
                        bool opposee_is_queen = int_to_bool(get_pawn_value(g, !color, indAmi, QUEEN));
                        putFriendByInd(g, indPawn, indAmi, color, false);
                        if (is_queen)
                            g->nbQueenWithFriend[color]--;
                        else
                            g->nbFriendNoQueen[color]--;

                        if (opposee_is_queen)
                            g->nbQueenWithFriend[!color]--;
                        else
                            g->nbFriendNoQueen[!color]--;
                    }
                }
            }
        }
        put_case_damier(g, i, j, IND_PAWN_ON_CASE, -1);
        /* Supprimer l'indice et optimiser la memoire */
        copy_remove_pawn_from_index_to_index(g, g->nb_pawns[color] - 1, indPawn, color);
        g->nb_pawns[color]--;
    }
    else
    {
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
    killPawn(g, i, j);
}

/* Put the pawn in a specific case (lig, col).
Useful for queen_move and can be used by us
to go faster for checks : place pawns where we want */

void change_pawn_place(Game *g, int ind, bool color, int lig, int col)
{
    put_case_damier(g, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL), IND_PAWN_ON_CASE, -1);
    put_pawn_value(g, color, ind, LIG, lig);
    put_pawn_value(g, color, ind, COL, col);
    put_case_damier(g, lig, col, IND_PAWN_ON_CASE, ind);
    put_case_damier(g, lig, col, PAWN_COLOR, color);
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

    int indVictim = get_case_damier(g, i + add0, j + add1).ind_pawn;
    // assert(ind > -1);
    change_pawn_place(g, indEater, color, i + 2 * add0, j + 2 * add1);

    // Kill pawn indVictim
    put_pawn_value(g, !color, indVictim, ALIVE, 0);
    put_case_damier(g, i + add0, j + add1, IND_PAWN_ON_CASE, VOID_INDEX);
    // printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return indVictim;
}

// promotes the indicated pawn, un ghost pawn ne peut pas devenir une dame
void promote(Game *g, bool is_white, int ind)
{
    put_pawn_value(g, is_white, ind, QUEEN, 1);
    if (has_friend(g, ind, is_white))
        g->nbQueenWithFriend[is_white]++;
    // Seul les pions qui ont eu des amis peuvent devenir des reines avec amitié
    else
        g->nbQueenWithoutFriend[is_white]++;
    int ind_ennemy = get_pawn_value(g, is_white, ind, ENNEMY);
    if (ind_ennemy != NEUTRAL_IND)
    {
        killPawn(g, get_pawn_value(g, !is_white, ind_ennemy, LIG), get_pawn_value(g, !is_white, ind_ennemy, COL));
    }
}

void createPawn(Game *g, bool color, int i, int j)
{
    // Rajoute un pion dans la liste des pions de la meme couleur. Attention taille du tableau
    assert(g->nb_pawns[color] < MAX_NB_PAWNS);
    int new_ind = g->nb_pawns[color];

    pawn_default_value(g, new_ind, color);
    // // Par securite, remet par defaut les valeurs pour creer le nouveau pion

    put_pawn_value(g, color, new_ind, ALIVE, 1);
    put_pawn_value(g, color, new_ind, LIG, i);
    put_pawn_value(g, color, new_ind, COL, j);

    put_case_damier(g, i, j, IND_PAWN_ON_CASE, new_ind);
    put_case_damier(g, i, j, PAWN_COLOR, color);

    // Just need to increment, we have enough place (only NB_PAWNS pawns to promote)
    g->nb_pawns[color]++;
}

void simplyPawnMove(Game *g, bool is_white, int ind, bool left)
{
    assertAndLog(isValidIndexInGame(g, ind, is_white), "simply pawn move non valid index");
    pawn p = get_pawn(g, is_white, ind);
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g, ind, is_white, i + di, j + dj);
}

void AleatStormBreaks(Game *g, bool color)
{
    int_chain *l = g->cloud[color];
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
            killPawnByInd(g, color, ind);
        }
    }
    g->lengthCloud[color] = 0;

    // C'est une fonction recursive car le pion foudroyer peut etre pres du nuage de la couleur opposee. On verifie donc
    // dans l'autre nuage

    // printf("%d %d %d %d", get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL), ind, color);
    // flush();
    if (ind != VOID_INDEX && canStormBreaksForTheOthers(g, ind, color))
        AleatStormBreaks(g, !color);
}

Coord CanMoveOrEatQueen(Game *g, bool color, int lig, int col, int ind, bool screen_switch)
{
    // Check if the move of the queen is possible and move the queen
    // If ennemy pawn is on the road, so queen will go just before it and then call Rafle, else it fails
    assertAndLog(inGame(lig, col), "Tuple queen pas dans damier");
    assertAndLog(isValidIndexInGame(g, ind, color), "Dame pas valide index");
    if (screen_switch)
    {
        lig = color ? NB_CASE_LG - lig - 1 : lig;
    }
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
            c = get_case_damier(g, plig + add_lig * i, pcol + add_col * i);
            if (!freeCase(c))
            {
                // Here the queen can eat a pawn
                int new_lig = add_lig + plig + add_lig * i;
                int new_col = add_col + pcol + add_col * i;
                // Check if the queen can go here
                if (c.pawn_color == !color && inGame(new_lig, new_col) && freeCase(get_case_damier(g, new_lig, new_col)) &&
                    !isInCloud(g, !color, ind_from_lig_col(g, new_lig - add_lig, new_col - add_col)))
                {
                    Coord pos_dame = {.i = new_lig, .j = new_col};
                    return pos_dame;
                }
                else if (c.pawn_color == color || (c.pawn_color != color && isInCloud(g, !color, c.ind_pawn)))
                {
                    printv("jump sheep");
                    if (!screen_switch)
                    {
                        // On est intransigeant avec la machine
                        assertAndLog(false, "");
                    }
                    Coord pos_dame = {.i = VOID_INDEX, .j = VOID_INDEX};
                    return pos_dame; // Pb: the queen jumps a pawn of her own color or a ghost pawn
                }
            }
        }
        // Only move the pawn: nothing in her path
        if (inGame(lig, col) && freeCase(get_case_damier(g, lig, col)))
        {
            Coord pos_dame = {.i = lig, .j = col};
            return pos_dame;
        }
    }
    printv("Le deplacement n'est pas diagonale");
    if (!screen_switch)
    {
        assertAndLog(false, "");
    }
    Coord pos_dame = {.i = VOID_INDEX, .j = VOID_INDEX};
    return pos_dame; // No case was found
}

// Memory Function
void free_game(Game *g)
{
    // Free the damier

    for (int i = 0; i < NB_CASE; i++)
    {
        free(g->damier[i]);
    }

    free(g->damier);

    for (int i = 0; i < MAX_NB_PAWNS; i++)
    {
        free(g->liensAmitie[i]);
    }

    free(g->liensAmitie);

    free(g->allPawns[0]);
    free(g->allPawns[1]);

    // free(g->allPawns);

    // free(g->lengthCloud);
    // free(g->nbFoe);
    // free(g->nbFriendNoQueen);
    // free(g->nbQueenWithFriend);
    // free(g->nbQueenWithoutFriend);
    // free(g->nb_pawns);

    freeIntChain(g->cloud[true]);
    freeIntChain(g->cloud[false]);
    // free(g->cloud);

    freeIntChain(g->inds_move_back);
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
    Coord init_coord_dame_rafle = {.i = get_pawn_value(g, iw, ind, LIG),
                                   .j = get_pawn_value(g, iw, ind, COL)};
    return init_coord_dame_rafle;
}

void doubleTabInit(int t[2])
{
    t[0] = 0;
    t[1] = 0;
}

void incrBothTab(int t[2])
{
    t[0]++;
    t[1]++;
}

void decrBothTab(int t[2])
{
    t[0]--;
    t[1]--;
}

Coord queen_valide_case(Game *g, int ind, bool color)
{
    int possibleShifts[2] = {-1, 1};
    Coord dir;
    Coord dir0 = coord_from_ind(g, ind, color);
    Coord arr;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            dir.i = possibleShifts[i];
            dir.j = possibleShifts[j];
            for (int k = NB_CASE_LG - 1; k > 0; k--)
            {
                arr = add(dir0, mult(k, dir));
                if (caseIsAccessible(g, g->is_white, arr.i, arr.j))
                {
                    bool is_diag_clean = true;
                    for (int l = 1; l < k; l++)
                    {
                        arr = add(dir0, mult(l, dir));
                        Case c = get_case_damier(g, arr.i, arr.j);
                        if (!freeCase(c) && (c.pawn_color == color || (c.pawn_color != color && isInCloud(g, !color, c.ind_pawn))))
                        {
                            is_diag_clean = false;
                            break;
                        }
                    }
                    if (is_diag_clean)
                    {
                        arr = add(dir0, mult(k, dir));
                        return arr;
                    }
                }
            }
        }
    }
    assertAndLog(false, "Dame : aucune case trouve");
    return dir;
}

void change_pawn_place_coord(Game *g, int ind, bool color, Coord pos)
{
    change_pawn_place(g, ind, color, pos.i, pos.j);
}

void stormBreaksNGE(Game *g, bool color, int index, memory_move_t *mem)
{
    // Liste chaine et valeur du pawn survivor modifies par effet de bord
    cloud_chain *load = mem->load_cloud;
    Coord pos_survivor = mem->issues[index].pos_survivor;
    int_chain *l = g->cloud[color];
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
            tcloud k = {.coord = {.i = get_pawn_value(g, color, ind, LIG),
                                  .j = get_pawn_value(g, color, ind, COL)},
                        .pba = get_pawn_value(g, color, ind, PBA)};
            cpush(load, k);
            killPawn(g, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL));
        }
    }
    g->lengthCloud[color] = 0;
    change_pawn_place_coord(g, ind, color, pos_survivor); // Dans le cloud, tous les pions ont
    // la même valeur donc on prend le dernier et on le bouge à la place du survivor
}

void initFriendsWhichMoveBack(memory_move_t *mem, int taille)
{
    // Initialise le tableau des amis qu'on enregistre
    assertAndLog(mem->friends_which_move_back == NULL, "initFriendsWhichMoveBack le tableau est deja rempli");
    mem->friends_which_move_back = malloc(taille * sizeof(Coord));
    for (int i = 0; i < taille; i++)
    {
        mem->friends_which_move_back[i].i = -1;
        mem->friends_which_move_back[i].j = -1;
    }
}

void cleanMemMoveBack(memory_move_t *mem)
{
    assertAndLog(mem->friends_which_move_back != NULL && mem->move_back_left_or_right != NULL, "cleanMemMoveBack : structures vides");
    free(mem->friends_which_move_back);
    assertAndLog(is_empty(mem->move_back_left_or_right), "cleanMemMoveBack: ils restent des gauches droites");
    freeIntChain(mem->move_back_left_or_right);
    mem->move_back_left_or_right = NULL;
    mem->friends_which_move_back = NULL;
}