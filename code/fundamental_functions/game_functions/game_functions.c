#include "game_functions.h"

/*







   Play functions






*/

// Elles ont toutes des effets de bord
// et on suppose que les coups joues sont legaux

// moves the indicated pawn frontward depending on the
// indicated direction.
// On suppose que ce coup est legal.
// Met fin au tour pour la structure de jeu

void pawnMoveBackNGE(Game *g, int ind, bool left)
{
    bool is_white = g->is_white;
    pawn p = get_pawn(g, is_white, ind);
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? 1 : - 1;

    change_pawn_place(g, ind, is_white, i - di, j - dj);
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
    pawnMoveBackNGE(g, indMovePawnBack, left);
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
    if (has_friend(g, ind, is_white)) {
    for (int i = 0; i < MAX_NB_PAWNS; i++)
    {
        if (isValidIndexInGame(g, i, !is_white) && getFriendByInd(g, ind, i, is_white) && !alreadyInList(g->inds_move_back, i))
        {
            push(g->inds_move_back, i);
        }
        // le pion indique a partir de son indice
    }}

    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
}

/*







   QUEEN FUNCTION






*/

void queenDepl(Game *g, int ind, bool color, queen_move_t tuple_coord)
{
    // Suppose que l'entree est valide
    bool doMoveBack = true;
    int lig = tuple_coord.pos_dame.i;
    int col = tuple_coord.pos_dame.j;
    int enn_lig = tuple_coord.pos_eaten_pawn.i;
    int enn_col = tuple_coord.pos_eaten_pawn.j;
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");
    ;
    change_pawn_place(g, ind, color, lig, col);
    if (enn_lig != -1 && enn_col != -1)
    {
        // In fact, this is useless because eatRafle do the job but it is satisfying to jump an ennemy pawn
        killPawn(g, enn_lig, enn_col);
        doMoveBack = false;
    }

    // Gonna check if the queen can take a rafle

    assert(g->currentTree == emptyTree);
    bool isWhite = g->is_white;
    g->currentTree = rafleTreeCalc(g, isWhite, g->ind_move);

    printf("lazyRafle called\n");
    Path *r = lazyRafle(g->currentTree);
    printf("eatRafle called\n");
    bool had_eaten = eatRafleNGM(g, g->ind_move, g->is_white, g->currentTree, r);
    if (doMoveBack)
        doMoveBack = !had_eaten;
    printf("pathFree called\n");
    pathFree(r);
    for (int i = 0; i < MAX_NB_PAWNS; i++)
    {
        if (isValidIndexInGame(g, i, !color) && getFriendByInd(g, ind, i, color) && alreadyInList(g->inds_move_back, i))
        {
            push(g->inds_move_back, i);
        }
        // le pion indique a partir de son indice
    }

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
    // Do a move back only if the queen didn't eat
}

/*







   Promotion functions






*/

void promotionPmetre(Game *g, bool is_white, int ind)
{
    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");
    ;
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    // int choice = rand() % 3;
    int choice = 2;
    if (choice == 1)
    {
        promote(g, is_white, ind);
        endTurnGameManagement(g, is_white, ind, IND_GLORY_QUEEN, false);
        return;
    }
    else if (choice == 2)
    {
        int i = get_pawn_value(g, is_white, ind, LIG);
        int j = get_pawn_value(g, is_white, ind, COL);

        // printf("Quantic %d %d", i, j);

        // Kill the former pawn
        killPawn(g, i, j);

        // Give birth to the ennemy pawn
        createPawn(g, !is_white, i, j);
        int indNew = get_case_damier(g, i, j).ind_pawn;
        if (canBePromoted(g, !is_white, indNew))
            promote(g, !is_white, indNew);

        endTurnGameManagement(g, is_white, indNew, IND_BAD_CHOICE, false);
        return;
    }
    endTurnGameManagement(g, is_white, ind, IND_NOTHING_HAPPENED, false);
}

void promotion(Game *g)
{
    promotionPmetre(g, g->is_white, g->ind_move);
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
    ;
    bool iw = g->is_white;
    lienAmitiePmetreNGE(lig, col, g->ind_move, iw, g);

    endTurnGameManagement(g, iw, g->ind_move, IND_CHANGE_ALLOWED, false);
}

/*







   MOVE BACK FUNCTION






*/

// Functions to move back a pawn because the friend has just moved

void moveBack(Game *g, bool autoplay, bool use_heuristique, float (*f)(Game *))
{
    /* Suppose move on the just pawn. Move back the pawn referred by inds_move_back to the case localised by the coord coordForMoveBack */

    bool iw = g->is_white;
    if (autoplay)
    {
        while (!is_empty(g->inds_move_back))
        {
            int ind = pop(g->inds_move_back);
            assertAndLog(isValidIndexInGame(g, ind, iw), "Pion sortie de la pile des amis non valide");
            bool recul_gauche = canMoveBack(g, iw, ind, true);
            bool recul_droite = canMoveBack(g, iw, ind, false);
            bool move_has_been_played = true;
            if (use_heuristique)
            {
                assertAndLog(false, "pas encore teste move Back heurist");

                float eval_gauche = 0;
                float eval_droite = 0;
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
                else
                {
                    move_has_been_played = false;
                }
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
                else
                {
                    move_has_been_played = false;
                }
            }

            if (move_has_been_played)
            {
                endTurnGameManagement(g, iw, ind, IND_CHANGE_ALLOWED, true);
            };
        }
    }
    else
    {
        assertAndLog(false, "Fuck it human, only machines play move back");
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
    ;
    bool iw = g->is_white;
    lienEnnemitiePmetreNGE(iw, lig, col, g->ind_move, g);
    endTurnGameManagement(g, iw, g->ind_move, IND_CHANGE_ALLOWED, false);
}

/*







   Deplacement quantique : bi-deplacement






*/

void biDepl(Game *g, int ind, bool color)
{
    // On suppose le coup legal
    bool depl = int_to_bool(rand() % 2);
    // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
    int dj = depl ? 1 : -1;

    assertAndLog(is_empty(g->inds_move_back), "Les amis sont toujours la");
    ;
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

    // Maybe the clone pawn is near a pawn of the opposite color
    if (canStormBreaks(g, newInd, color))
        AleatStormBreaks(g, color);
    else if (canStormBreaksForTheOthers(g, newInd, color))
        AleatStormBreaks(g, !color);

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
}
