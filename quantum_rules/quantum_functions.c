#include "quantum_functions.h"



/*







    LOGIC FUNCTIONS






*/

bool moveBackAvailable(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i > VOID_INDEX && g->coordForMoveBack.j > VOID_INDEX;
}

bool needPutMoveBack(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i == IND_LISTENING_MOVE_BACK && g->coordForMoveBack.j == IND_LISTENING_MOVE_BACK;
}

bool canBeFriend(pawn p, Case c, pawn Npawns[])
{
    // Check if a pawn and the pawn on the case c can be friend
    return !freeCase(c) && c.pawn_color != p.color && Npawns[c.ind_pawn].friendly == -1 && p.friendly == -1 && p.ennemy == -1 && Npawns[c.ind_pawn].ennemy == -1;
}

bool canBeEnnemy(pawn p, Case c, pawn Npawns[])
{
    return canBeFriend(p, c, Npawns);
}


/*







   MANAGEMENT FUNCTIONS






*/


void moveBackGameManagement(Game *g)
{
    // Remet par defaut les differents indices apres un mouvement arriere du pion
    g->ind_move_back = VOID_INDEX;
}


/*







   Promotion functions






*/


void promotionPmetre(pawn pawns[], pawn Npawns[], bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    assert(ind > -1 && pawns[ind].alive);
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
        killPawn(pawns, Npawns, damier, i, j);
        // Give birth to the ennemy pawn
        int new_ind = g->nb_pawns[!is_white];

        pawn_default_value(Npawns, new_ind, !is_white);
        // Par securite, remet par defaut les valeurs pour creer le nouveau pion

        Npawns[new_ind].alive = true;
        Npawns[new_ind].col = j;
        Npawns[new_ind].lig = i;

        g->damier[i][j].ind_pawn = new_ind;
        g->damier[i][j].pawn_color = !is_white;

        // Just need to increment, we have enough place (only NB_PAWNS pawns to promote)
        g->nb_pawns[!is_white]++;
        endTurnGameManagement(g, is_white, ind, IND_BAD_CHOICE, false);
        return;
    }
    endTurnGameManagement(g, is_white, ind, IND_NOTHING_HAPPENED, false);
}

void promotion(Game *g)
{
    promotionPmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], g->is_white, g->damier, g->ind_move, g);
}

/* A implementer : Gestion des suppressions d'indice pour combler les trous, l'ordre des pions n'a pas d'importance */


/*







   LienAmitie functions






*/

void lienAmitiePmetre(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool is_white, Game *g)
{
    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Pawn.friendly. Si on lie d'amitié un pion qui était déjà ami, l'action n'a pas lieu et
    le joueur doit rejouer.
    Suppose le pion ainsi que le pion ami selectionne valides */
    Case c = damier[lig][col];
    put_pawn_value(g, is_white, ind, 3, false, -1, c.ind_pawn, false, -1, -1);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 3, false, -1, ind, false, -1, -1);
    endTurnGameManagement(g, is_white, ind, IND_CHANGE_ALLOWED, false);
}

void lienAmitie(int lig, int col, Game *g)
{
    // On suppose le coup legal
    bool iw = g->is_white;
    lienAmitiePmetre(g->allPawns[iw], g->allPawns[!iw], lig, col, g->damier, g->ind_move, iw, g);
}

// Functions to move back a pawn because the friend has just moved

bool canMoveBack(Game *g, bool is_white, int ind, bool left)
{
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {g->allPawns[is_white][ind].lig - di,
                      g->allPawns[is_white][ind].col + dj};
    bool posInGame = inGame(finalPos.i, finalPos.j);
    bool wayIsFree = freeCase(g->damier[finalPos.i][finalPos.j]);
    return posInGame && wayIsFree;
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


void lienEnnemitiePmetre(bool is_white, pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friendly. Si on declare ennemi un pion qui était déjà ennemi, le coup n'est pas joué
    Suppose legal move */
    Case c = damier[lig][col];
    put_pawn_value(g, is_white, ind, 2, false, c.ind_pawn, -1, false, -1, -1);
    put_pawn_value(g, c.pawn_color, c.ind_pawn, 2, false, ind, -1, false, -1, -1);
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
    lienEnnemitiePmetre(iw, g->allPawns[iw], g->allPawns[!iw], lig, col, g->damier, g->ind_move, g);
}