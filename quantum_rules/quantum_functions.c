#include "quantum_functions.h"

// Aux functions

// int changeForMoveBack(Game *g, pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col)
// {
//     change_pawn_place(pawns, damier, ind, lig, col);
//     g->ind_move_back = -1;
//     return IND_BACK_ALLOWED;
// }

bool moveBackNotAvailable(Game *g)
{
    return (g->coordForMoveBack.i == -1 && g->coordForMoveBack.j == -1);
}

// Promotion functions

int promotionPmetre(pawn pawns[], pawn Npawns[], bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    assert(ind > -1 && pawns[ind].alive);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    int choice = rand() % 3;
    if (choice == 1)
    {
        pawns[ind].queen = true;
        return IND_GLORY_QUEEN;
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
        return IND_BAD_CHOICE;
    }
    return IND_NOTHING_HAPPENED;
}

/* Gestion des suppressions d'indice pour combler les trous, l'ordre des pions n'a pas d'importance */

void promotion(Game *g)
{
    if (g->ind_move > -1)
        g->indCheck = promotionPmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], g->is_white, g->damier, g->ind_move, g);
    else
        g->indCheck = IND_PB;
    g->ind_move = -1;
}

// LienAmitie functions

bool canBeFriend(pawn p, Case c)
{
    // Check if a pawn and the pawn on the case c can be friend
    return !freeCase(c) && c.pawn_color != p.color;
}

int lienAmitiePmetre(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool is_white)
{
    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Pawn.friendly. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
    assert(ind > VOID_INDEX && pawns[ind].alive);
    if (is_white)
        lig = NB_CASE_LG - lig - 1;

    Case c = damier[lig][col];
    if (canBeFriend(pawns[ind], c))
    {
        pawns[ind].friendly = c.ind_pawn;
        Npawns[c.ind_pawn].friendly = ind;
        return IND_CHANGE_ALLOWED;
    }
    return IND_PB;
}

void lienAmitie(int col, int lig, Game *g)
{
    assert(g->ind_move > -1);
    bool iw;
    g->indCheck = lienAmitiePmetre(g->allPawns[iw], g->allPawns[!iw], lig, col, g->damier, g->ind_move, iw);
    g->ind_move = -1;
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
    int ind = g->ind_move;
    bool is_white = g->is_white;
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;

    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    
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
    /* Move back the pawn referred by ind_move_back to the case localised by the coord coordForMoveBack */
    int ind = g->ind_move_back;
    bool iw;
    pawn p = g->allPawns[iw][ind];
    assert(!moveBackNotAvailable(g) && !outOfBounds(g->coordForMoveBack.i, g->coordForMoveBack.j) && ind > -1 && p.alive);
    change_pawn_place(g->allPawns[iw], g->damier, ind, g->coordForMoveBack.i, g->coordForMoveBack.j);
    g->coordForMoveBack.i = -1;
    g->coordForMoveBack.j = -1;
    g->ind_move_back = -1;
}

// LienEnnemitie functions

bool canBeEnnemy(pawn p, Case c)
{
    return !freeCase(c) && c.pawn_color != p.color;
}

int lienEnnemitiePmetre(bool is_white, pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friendly. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */
    assert(ind > -1 && pawns[ind].alive);
    if (is_white)
        lig = NB_CASE_LG - lig - 1;
    Case c = damier[lig][col];
    if (canBeEnnemy(pawns[ind], c))
    {
        pawns[ind].ennemy = c.ind_pawn;
        Npawns[c.ind_pawn].ennemy = ind;
        return IND_CHANGE_ALLOWED;
    }
    return IND_PB;
}

void lienEnnemitie(int col, int lig, Game *g)
{
    assert(g->ind_move > -1);
    bool iw;
    g->indCheck = lienEnnemitiePmetre(iw, g->allPawns[iw], g->allPawns[!iw], lig, col, g->damier, g->ind_move);
    g->ind_move = -1;
}