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
    startTurnGameManagement(g);
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
    if (p->friendly != NEUTRAL_IND)
    {
        g->ind_move_back = p->friendly;
        // implementer une fonction qui se charge de faire reculer
        // le pion indique a partir de son indice
    }
    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, true);
}

/*







   MOVE BACK FUNCTION






*/

// Queen functions

/*







   Promotion functions






*/

void promotionPmetre(pawn pawns[], bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    startTurnGameManagement(g);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    int choice = rand() % 3;
    if (choice == 1)
    {
        pawns[ind].queen = true;
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

void lienAmitiePmetre(int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool is_white, Game *g)
{
    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Pawn.friendly. Si on lie d'amitié un pion qui était déjà ami, l'action n'a pas lieu et
    le joueur doit rejouer.
    Suppose le pion ainsi que le pion ami selectionne valides */
    startTurnGameManagement(g);
    Case c = damier[lig][col];
    put_pawn_value(g, is_white, ind, 3, c.ind_pawn);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 3, ind);
    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
}

void lienAmitie(int lig, int col, Game *g)
{
    // On suppose le coup legal
    bool iw = g->is_white;
    lienAmitiePmetre(lig, col, g->damier, g->ind_move, iw, g);
}

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

void lienEnnemitiePmetre(bool is_white, int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friendly. Si on declare ennemi un pion qui était déjà ennemi, le coup n'est pas joué
    Suppose legal move */
    startTurnGameManagement(g);
    Case c = damier[lig][col];
    put_pawn_value(g, is_white, ind, 2, c.ind_pawn);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 2, ind);
    // printf("Enn %d %d\n", lig, col);
    // print_pawn(g->allPawns[c.pawn_color][c.ind_pawn], c.ind_pawn);
    // printf("Actu pawn");
    // print_pawn(g->allPawns[is_white][ind], ind);
    // flush();
    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
}

void lienEnnemitie(int lig, int col, Game *g)
{
    // Suppose legal move
    bool iw = g->is_white;
    lienEnnemitiePmetre(iw, lig, col, g->damier, g->ind_move, g);
}

/*







   Deplacement quantique : bi-deplacement






*/

void biDepl(Game *g, int ind, bool color)
{
    // On suppose le coup legal

    startTurnGameManagement(g);
    int di = color ? 1 : -1;
    // Creer un pion a droite
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + 1;
    createPawn(g, color, newLig, newCol);
    int newInd = g->damier[newLig][newCol].ind_pawn;
    put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

    // Rajoute dans le cloud
    if (!isInCloud(g, color, ind))
        push(g->cloud[color], ind);
    push(g->cloud[color], newInd);

    g->lengthCloud[color]++;

    // Deplace le pion a gauche
    put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
    simplyPawnMove(g, color, ind, true);

    // Maybe the clone pawn is near a pawn of the opposite color
    if (canStormBreaks(g, newInd, color)) AleatStormBreaks(g, color);
    else if (canStormBreaksForTheOthers(g, newInd, color)) AleatStormBreaks(g, !color); 

    endTurnGameManagement(g, color, ind, IND_CHANGE_ALLOWED, false);
}
