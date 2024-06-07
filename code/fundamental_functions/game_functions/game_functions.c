#include "game_functions.h"

/*







   Play functions






*/

// Elles ont toutes des effets de bord
// et on suppose que les coups joues sont legaux

void cancelAllMoveBack(Game *g, memory_move_t *mem)
{
    assertAndLog(mem->friends_which_move_back != NULL && !is_empty(mem->move_back_left_or_right), "cancelAllMoveBack : structures vides");
    bool iw = mem->is_white;
    int ind = -1;
    int taille = taille_list(mem->move_back_left_or_right);
    for (int i = taille - 1; i > -1; i--) // C'est important pour sortir de la pile
    {

        ind = ind_from_coord(g, mem->friends_which_move_back[i]);
        int left = pop(mem->move_back_left_or_right);
        if (left != NO_MOVE)
        {
            bool turn_left = left == LEFT ? true : false;
            simplyPawnMove(g, iw, ind, !turn_left); // Sens opposé
        }
    }
    cleanMemMoveBack(mem);
}

void cancelMoveBack(Game *g, int ind, bool left)
{
    bool iw = g->is_white;
    simplyPawnMove(g, iw, ind, left);
}

void lazzyMoveBack(Game *g, int indMovePawnBack, bool left)
{
    // On considère le pire cas, si un pion s'approche d'un fantome de couleur opposée, il le considère
    // de pba 1
    assertAndLog(isValidIndexInGame(g, indMovePawnBack, g->is_white), "lazzyMoveBack ind non valide");
    bool is_white = g->is_white;
    pawn p = g->allPawns[is_white][indMovePawnBack];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? 1 : -1;

    change_pawn_place(g, indMovePawnBack, is_white, i - di, j - dj);
}

void cancelLazzyMoveBack(Game *g, int indMovePawnBack, bool left)
{
    assertAndLog(isValidIndexInGame(g, indMovePawnBack, g->is_white), "cancellazzyMoveBack ind non valide");

    cancelMoveBack(g, indMovePawnBack, left);
}

void pawnMove(Game *g, bool is_white, int ind, bool left)
{
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    int i = get_pawn_value(g, is_white, ind, LIG);
    int j = get_pawn_value(g, is_white, ind, COL);
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g, ind, is_white, i + di, j + dj);
    if (has_friend(g, ind, is_white))
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

    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
}

/*







   QUEEN FUNCTION






*/

// void queenDeplHuman(Game *g, int ind, bool color, queen_move_t tuple_coord)
// {
//     // Suppose que l'entree est valide
//     bool doMoveBack = true;
//     int lig = tuple_coord.pos_dame.i;
//     int col = tuple_coord.pos_dame.j;
//     int enn_lig = tuple_coord.pos_eaten_pawn.i;
//     int enn_col = tuple_coord.pos_eaten_pawn.j;
//     assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

//     change_pawn_place(g, ind, color, lig, col);
//     if (enn_lig != -1 && enn_col != -1)
//     {
//         // In fact, this is useless because eatRafle do the job but it is satisfying to jump an ennemy pawn
//         killPawn(g, enn_lig, enn_col);
//         doMoveBack = false;
//     }

//     // Gonna check if the queen can take a rafle

//     assert(g->currentTree == emptyTree);
//     bool isWhite = g->is_white;
//     g->currentTree = rafleTreeCalc(g, isWhite, g->ind_move);

//     printf("lazyRafle called\n");
//     Path *r = lazyRafle(g->currentTree);
//     printf("eatRafle called\n");
//     bool had_eaten = eatRafleNGM(g, g->ind_move, g->is_white, g->currentTree, r);
//     if (doMoveBack)
//         doMoveBack = !had_eaten;
//     printf("pathFree called\n");
//     pathFree(r);
//     if (doMoveBack)
//     {
//         for (int i = 0; i < MAX_NB_PAWNS; i++)
//         {
//             if (isValidIndexInGame(g, i, !color) && getFriendByInd(g, ind, i, color) && !alreadyInList(g->inds_move_back, i))
//             {
//                 push(g->inds_move_back, i);
//             }
//             // le pion indique a partir de son indice
//         }
//     }

//     endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
//     // Do a move back only if the queen didn'l eat
// }

void queenDepl(Game *g, int ind, bool color, Coord pos_dame, bool isNGE)
{
    /* Selon qu'on souhaite des effets graphiques ou pas, on appelle rafleNGE ou rafle
    et on applique endTurnGameManagement */
    // Suppose que l'entree est valide
    bool doMoveBack = true;
    int lig = pos_dame.i;
    int col = pos_dame.j;
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    change_pawn_place(g, ind, color, lig, col);

    // Gonna check if the queen can take a rafle

    assertAndLog(g->currentTree == emptyTree, "arbre de g non vide");
    bool isWhite = g->is_white;

    if (!isNGE)
    {
        // si le coup est réelle, on peut avoir une rafle à la fin
        g->currentTree = rafleTreeCalc(g, isWhite, g->ind_move);

        Path *r = lazyRafle(g->currentTree);
        bool had_eaten = eatRafleNGM(g, g->ind_move, g->is_white, g->currentTree, r);
        doMoveBack = !had_eaten;
        pathFree(r);
    }
    // Sinon la rafle de la dame est traité dans ralfeDeter

    if (doMoveBack && has_friend(g, ind, color))
    {
        for (int i = 0; i < MAX_NB_PAWNS; i++)
        {
            if (isValidIndexInGame(g, i, !color) && getFriendByInd(g, ind, i, color) && !alreadyInList(g->inds_move_back, i))
            {
                push(g->inds_move_back, i);
            }
            // le pion indique a partir de son indice
        }
    }

    if (!isNGE)
    {
        endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
        // Do a move back only if the queen didn'l eat
    }
}

void queenDeplDueMoveStruct(Game *g, int ind, bool color, Coord pos_dame)
{
    /* Déplace la reine à partir d'un move, donc c'est un simple déplacement sans rafle */
    // Suppose que l'entree est valide
    int lig = pos_dame.i;
    int col = pos_dame.j;
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    change_pawn_place(g, ind, color, lig, col);

    // Gonna check if the queen can take a rafle

    assertAndLog(g->currentTree == emptyTree, "arbre de g non vide");

    if (has_friend(g, ind, color))
    {
        for (int i = 0; i < MAX_NB_PAWNS; i++)
        {
            if (isValidIndexInGame(g, i, !color) && getFriendByInd(g, ind, i, color) && !alreadyInList(g->inds_move_back, i))
            {
                push(g->inds_move_back, i);
            }
            // le pion indique a partir de son indice
        }
    }

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
    // Do a move back only if the queen didn'l eat
}

/*







   Promotion functions






*/

void promotion(Game *g)
{
    Coord pos = promotionNGE(g, VOID_INDEX, g->is_white);
    if (pos.i != -1)
    {
        assert(pos.j != -1);
        int indNew = ind_from_coord(g, pos);
        endTurnGameManagement(g, g->is_white, indNew, IND_BAD_CHOICE, false);
    }
    else
    {
        if (pos.j == IND_GLORY_QUEEN)
        {
            endTurnGameManagement(g, g->is_white, g->ind_move, IND_GLORY_QUEEN, false);
        }
        else
        {
            assert(pos.j == IND_NOTHING_HAPPENED);
            endTurnGameManagement(g, g->is_white, g->ind_move, IND_NOTHING_HAPPENED, false);
        }
    }
}

/*







   LienAmitie functions






*/

void lienAmitie(int lig, int col, Game *g)
{
    // On suppose le coup legal

    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Pawn.friendly. Si on lie d'amitié un pion qui était déjà ami, l'action n'a pas lieu et
    le joueur doit rejouer.
    Suppose le pion ainsi que le pion ami selectionne valides */
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    bool iw = g->is_white;
    lienAmitieNGE(lig, col, g->ind_move, iw, g);

    endTurnGameManagement(g, iw, g->ind_move, IND_CHANGE_ALLOWED, false);
}

/*







   MOVE BACK FUNCTION






*/

// Play back and return if the pawn has moved or not
void moveBackNGE(Game *g, bool autoplay, bool use_heuristique, double (*f)(Game *), memory_move_t *mem)
{
    // mem vaut NULL lors du jeu avec effet graphique
    bool iw = g->is_white;
    if (autoplay)
    {
        int_chain *l;
        int taille_l;
        bool isNGE = mem != NULL;
        if (isNGE)
        {
            l = mem->inds_move_back;
            taille_l = taille_list(l);
            initFriendsWhichMoveBack(mem, taille_l);
        }
        else
        {
            l = g->inds_move_back;
            taille_l = taille_list(l);
        }
        for (int i = 0; i < taille_l; i++)
        {
            int ind = get(l, i);
            assertAndLog(isValidIndexInGame(g, ind, iw), "Pion sortie de la pile des amis non valide");

            if (!isNGE)
            { // On joue sur le moment et on évalue avec une heuristique ou non le meilleur coup possible
                bool recul_gauche = canMoveBack(g, iw, ind, true);
                bool recul_droite = canMoveBack(g, iw, ind, false);
                if (use_heuristique)
                {
                    assertAndLog(false, "pas encore teste move Back heurist");

                    double eval_gauche = 0;
                    double eval_droite = 0;
                    if (recul_droite && recul_gauche)
                    {
                        lazzyMoveBack(g, ind, true);
                        eval_gauche = f(g);
                        cancelLazzyMoveBack(g, ind, true);

                        lazzyMoveBack(g, ind, false);
                        eval_droite = f(g);
                        if (eval_droite <= eval_gauche)
                        {
                            cancelLazzyMoveBack(g, ind, false);
                            lazzyMoveBack(g, ind, true);
                        }
                    }
                    else if (recul_gauche)
                    {
                        lazzyMoveBack(g, ind, true);
                    }
                    else if (recul_droite)
                    {
                        lazzyMoveBack(g, ind, false);
                    }
                    // Sinon rien ne se passe
                }
                else
                {
                    if (recul_gauche)
                    {
                        lazzyMoveBack(g, ind, true);
                    }
                    else if (recul_droite)
                    {
                        lazzyMoveBack(g, ind, false);
                    }
                    // Sinon rien ne se passe
                }
            }
            else
            {
                switch (mem->move_back_left_or_right->tableau[i])
                {
                case LEFT:
                    lazzyMoveBack(g, ind, true);
                    break;

                case RIGHT:
                    lazzyMoveBack(g, ind, false);
                    break;

                case NO_MOVE:
                    break;

                default:
                    assertAndLog(false, "moveBackNGE move_back_left_or_right pas correspondant");
                    break;
                }

                // on enregistre le coord du pion qui recule
                int lig = get_pawn_value(g, iw, ind, LIG);
                int col = get_pawn_value(g, iw, ind, COL);
                mem->friends_which_move_back[i].i = lig;
                mem->friends_which_move_back[i].j = col;
            }
        }
    }
    else
    {
        assertAndLog(false, "Fuck it human, only machines play move back");
    }
}

// Functions to move back a pawn because the friend has just moved

void moveBack(Game *g, bool autoplay, bool use_heuristique, double (*f)(Game *))
{
    /* Suppose move on the just pawn. Move back the pawn referred by inds_move_back to the case localised by the coord coordForMoveBack */
    moveBackNGE(g, autoplay, use_heuristique, f, NULL);
    int ind;
    while (!is_empty(g->inds_move_back))
    {
        ind = pop(g->inds_move_back);
        endTurnGameManagement(g, g->is_white, ind, IND_CHANGE_ALLOWED, true);
    }
}

/*







   LienEnnemitie functions






*/

void lienEnnemitie(int lig, int col, Game *g)
{
    // Suppose legal move

    /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friendly. Si on declare ennemi un pion qui était déjà ennemi, le coup n'est pas joué
    Suppose legal move */
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");

    bool iw = g->is_white;
    lienEnnemitieNGE(iw, lig, col, g->ind_move, g);
    endTurnGameManagement(g, iw, g->ind_move, IND_CHANGE_ALLOWED, false);
}

/*







   Deplacement quantique : bi-deplacement






*/

void biDepl(Game *g, int ind, bool color)
{
    coord_bool_t data = biDeplNGE(g, color, ind);
    int newInd = ind_from_coord(g, data.c);
    // Maybe the clone pawn is near a pawn of the opposite color
    if (canStormBreaks(g, newInd, color))
    {
        newInd = AleatStormBreaks(g, color);
        endTurnGameManagementNGE(g, newInd, IND_NORMAL, false);
    }
    if (canStormBreaksForTheOthers(g, newInd, color))
    {
        int indEnn = AleatStormBreaks(g, !color);
        endTurnGameManagementNGE(g, indEnn, IND_NORMAL, false);
    }

    if (isValidIndexInGame(g, ind, color))
    {
        // Le pion ind a pu être tué lors de l'éclatement du nuage
        endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
    }
    else
    {
        // Dans ce cas là on appelle avec newInd, qui a déjà été appelé mais c'est pas grave
        endTurnGameManagement(g, color, newInd, IND_CHANGE_ALLOWED, false);
    }
}
