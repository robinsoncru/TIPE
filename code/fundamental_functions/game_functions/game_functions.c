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

void pawnMove(Game *g, bool is_white, int ind, bool left)
{
    startTurnGameManagement(g); // C'est ici qu'il y a corruption du paramètre
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
    if (p->friendly != NEUTRAL_IND)
    {
        g->ind_move_back = p->friendly;
        // le pion indique a partir de son indice
    }
    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, true);
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
    startTurnGameManagement(g);
    change_pawn_place_new(g, g->damier, ind, color, lig, col);
    if (enn_lig != -1 && enn_col != -1)
    {
        // In fact, this is useless because eatRafle do the job but it is satisfying to jump an ennemy pawn
        killPawn(g, g->damier, enn_lig, enn_col);
        doMoveBack = false;
    }

    // Gonna check if the queen can take a rafle

    assert(g->currentTree == emptyTree);
    bool isWhite = g->is_white;
    pawn *pawns = g->allPawns[isWhite];
    pawn *NPawns = g->allPawns[!isWhite];
    g->currentTree = rafleTreeCalc(pawns, NPawns, g->damier, g->ind_move);

    printf("lazyRafle called\n");
    Path *r = lazyRafle(g->currentTree);
    printf("eatRafle called\n");
    bool had_eaten = eatRafleNGM(g, g->ind_move, g->is_white, g->currentTree, r);
    if (doMoveBack)
        doMoveBack = !had_eaten;
    printf("pathFree called\n");
    pathFree(r);

    if (get_pawn_value(g, color, ind, FRIENDLY) != NEUTRAL_IND && doMoveBack)
    {
        g->ind_move_back = get_pawn_value(g, color, ind, FRIENDLY);
        // le pion indique a partir de son indice
    }
    else
        g->ind_move_back = VOID_INDEX;

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, doMoveBack);
    // Do a move back only if the queen didn't eat
}

/*







   Promotion functions






*/

void promotionPmetre(pawn *pawns, bool is_white, Case **damier, int ind, Game *g)
{
    startTurnGameManagement(g);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    int choice = rand() % 3;
    if (choice == 1)
    {
        promote(g, is_white, ind);
        endTurnGameManagement(g, is_white, ind, IND_GLORY_QUEEN, false);
        return;
    }
    else if (choice == 2)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("Quantic %d %d", i, j);

        // Kill the former pawn
        killPawn(g, damier, i, j);

        // Give birth to the ennemy pawn
        createPawn(g, !is_white, i, j);
        int indNew = damier[i][j].ind_pawn;
        if (canBePromoted(g, !is_white, indNew))
            promote(g, !is_white, indNew);

        endTurnGameManagement(g, is_white, ind, IND_BAD_CHOICE, false);
        return;
    }
    endTurnGameManagement(g, is_white, ind, IND_NOTHING_HAPPENED, false);
}

void promotion(Game *g)
{
    promotionPmetre(g->allPawns[g->is_white], g->is_white, g->damier, g->ind_move, g);
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
    startTurnGameManagement(g);
    bool iw = g->is_white;
    lienAmitiePmetreNGE(lig, col, g->damier, g->ind_move, iw, g);

    endTurnGameManagement(g, iw, g->ind_move, IND_CHANGE_ALLOWED, false);
}

/*







   MOVE BACK FUNCTION






*/

// Functions to move back a pawn because the friend has just moved

void moveBack(Game *g)
{
    /* Suppose move on the just pawn. Move back the pawn referred by ind_move_back to the case localised by the coord coordForMoveBack */
    int ind = g->ind_move_back;
    bool iw = g->is_white;
    change_pawn_place(g->allPawns[iw], g->damier, ind, g->coordForMoveBack.i, g->coordForMoveBack.j);
    moveBackGameManagement(g);
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
    startTurnGameManagement(g);
    bool iw = g->is_white;
    lienEnnemitiePmetreNGE(iw, lig, col, g->damier, g->ind_move, g);
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

    startTurnGameManagement(g);
    int di = color ? 1 : -1;
    // Creer un pion a droite ou a gauche aleatoirement
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + dj;
    createPawn(g, color, newLig, newCol);
    int newInd = g->damier[newLig][newCol].ind_pawn;
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

    // Maybe the clone pawn is near a pawn of the opposite color
    if (canStormBreaks(g, newInd, color))
        AleatStormBreaks(g, color);
    else if (canStormBreaksForTheOthers(g, newInd, color))
        AleatStormBreaks(g, !color);

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
}
