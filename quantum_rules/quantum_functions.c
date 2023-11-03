#include "quantum_functions.h"

// Aux functions

// int changeForMoveBack(Game *g, pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col)
// {
//     change_pawn_place(pawns, damier, ind, lig, col);
//     g->ind_move_back = -1;
//     return IND_BACK_ALLOWED;
// }

bool moveBackNotAvailable(Game *g) {
    return (g->coordForMoveBack.i == -1 && g->coordForMoveBack.j == -1);
}

// Promotion functions

void promotionPmetre(pawn pawns[], pawn Npawns[], bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    assert(ind > -1 && pawns[ind].alive);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    int choice = rand() % 3;
    if (choice == 1)
    {
        pawns[ind].queen = true;
        g->ind_check = IND_GLORY_QUEEN;
    }
    else if (choice == 2)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("Quantic %d %d", i, j);
        // Kill the former pawn
        popPawn(pawns, Npawns, damier, i, j);
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
        g->ind_check = IND_BAD_CHOICE;
    }
    else
    {
        g->ind_check = IND_NOTHING_HAPPENED;
    }
}

/* Gestion des suppressions d'indice pour combler les trous, l'ordre des pions n'a pas d'importance */

void promotion(Game *g)
{
    if (g->ind_move > -1)
        promotionPmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], g->is_white, g->damier, g->ind_move, g);
    else
        g->ind_check = IND_PB;
    g->ind_move = -1;
}

// LienAmitie functions

bool lienAmitiePmetre(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Pawn.friend. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
    assert(ind > -1 && pawns[ind].alive);
    Case c = damier[lig][col];
    if (!freeCase(c) && c.pawn_color != pawns[ind].color)
    {
        pawns[ind].friend = c.ind_pawn;
        Npawns[c.ind_pawn].friend = ind;
        return true;
    }
    return false;
}

void lienAmitie(int col, int lig, Game *g)
{
    assert(g->ind_move > -1);
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;
    if (lienAmitiePmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], lig, col, g->damier, g->ind_move))
    {
        g->ind_check = IND_CHANGE_ALLOWED;
    }
    else
        g->ind_check = IND_PB;
    g->ind_move = -1;
}

Coord CanMoveBackPmetre(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche)
{
    /* Give the coord of the case where moves back a pawn when it is linked with a friend and the friend has just moved. Return (-1, -1)
    if no case available */
    Coord coordForMoveBack = {-1, -1};
    if (ind > -1 && pawns[ind].alive)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        if (pawns[ind].color && i > 0)
        {
            if (gauche && j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                coordForMoveBack.i = i - 1;
                coordForMoveBack.j = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                coordForMoveBack.i = i - 1;
                coordForMoveBack.j = j + 1;
            }
            else if (j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                coordForMoveBack.i = i - 1;
                coordForMoveBack.j = j - 1;
            }
            else if (j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                coordForMoveBack.i = i - 1;
                coordForMoveBack.j = j + 1;
            }
            // Sinon aucun recule vers l'arrière n'est possible
        }
        else if (!pawns[ind].color && i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                coordForMoveBack.i = i + 1;
                coordForMoveBack.j = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                coordForMoveBack.i = i + 1;
                coordForMoveBack.j = j + 1;
            }
            else if (j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                coordForMoveBack.i = i + 1;
                coordForMoveBack.j = j - 1;
            }
            else if (j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                coordForMoveBack.i = i + 1;
                coordForMoveBack.j = j + 1;
            }
            // Sinon aucun recule vers l'arrière n'est possible
        }
    }
    // printf("pawn alive %d or ind = %d", pawns[ind].alive, ind);
    return coordForMoveBack;
}

void canMoveBack(bool gauche, Game *g) {
    Coord coordReturn = CanMoveBackPmetre(g->allPawns[g->is_white], g->damier, g->ind_move_back, gauche);
    g->coordForMoveBack.i = coordReturn.i;
    g->coordForMoveBack.j = coordReturn.j;
}

void moveBack(Game *g) {
    int ind = g->ind_move_back;
    pawn p = g->allPawns[g->is_white][ind];
    assert(!moveBackNotAvailable(g) && !outOfBounds(g->coordForMoveBack.i, g->coordForMoveBack.j) &&  ind > -1 && p.alive);
    change_pawn_place(g->allPawns[g->is_white], g->damier, ind, g->coordForMoveBack.i, g->coordForMoveBack.j);
    g->coordForMoveBack.i = -1;
    g->coordForMoveBack.j = -1;
    g->ind_move_back = -1;
}

// LienEnnemitie functions

bool lienEnnemitiePmetre(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    /* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */
    assert(ind > -1 && pawns[ind].alive);
    Case c = damier[lig][col];
    if (!freeCase(c) && c.pawn_color != pawns[ind].color)
    {
        pawns[ind].ennemy = c.ind_pawn;
        Npawns[c.ind_pawn].ennemy = ind;
        return true;
    }
    return false;
}

void lienEnnemitie(int col, int lig, Game *g)
{
    assert(g->ind_move > -1);
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;
    if (lienEnnemitiePmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], lig, col, g->damier, g->ind_move))
    {
        g->ind_check = IND_CHANGE_ALLOWED;
    }
    else
        g->ind_check = IND_PB;
    g->ind_move = -1;
}