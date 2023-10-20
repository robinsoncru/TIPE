#include "quantum_functions.h"

void promotionPmetre(pawn pawns[], pawn Npawns[], bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, Game *g)
{
    assert(ind > -1);
    /* Promote the pawn at the ind in pmetre : do nothing, become a queen or become an ennemy pawn */
    int choice = rand() % 3;
    if (choice == 1)
    {
        pawns[ind].queen = true;
        g->ind_move = IND_GLORY_QUEEN;
    }
    else if (choice == 2)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("Quantic %d %d", i, j);
        // Kill the former pawn
        popPawn(pawns, damier, i, j);
        // Give birth to the ennemy pawn
        int new_ind = g->nb_pawns[!is_white];

        Npawns[new_ind].alive = true;
        Npawns[new_ind].col = j;
        Npawns[new_ind].lig = i;
        Npawns[new_ind].color = !is_white;
        Npawns[new_ind].queen = false;

        g->damier[i][j].ind_pawn = new_ind;
        g->damier[i][j].pawn_color = !is_white;

        // Just need to increment, we have enough place (only NB_PAWNS pawns to promote)
        g->nb_pawns[!is_white]++;
        g->ind_move = IND_BAD_CHOICE;
    }
    else
    {
        g->ind_move = IND_NOTHING_HAPPENED;
    }
}

/* Gestion des suppressions d'indice pour combler les trous, l'ordre des pions n'a pas d'importance */

void promotion(Game *g)
{
    if (g->ind_move > -1)
        promotionPmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], g->is_white, g->damier, g->ind_move, g);
    else
        g->ind_move = IND_PB;
}

bool lienAmitiePmetre(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    /* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
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
    if (g->is_white)
        lig = NB_CASE_LG - lig - 1;
    if (g->declare_amis && lienAmitiePmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], lig, col, g->damier, g->ind_move))
    {
        g->ind_move = IND_CHANGE_ALLOWED;
        g->declare_amis = false;
    }
    else
        g->ind_move = IND_PB;
}

int moveBackPmetre(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche)
{
    /* Move back a pawn when it is linked with a friend and the friend has just moved */
    if (ind > -1 && pawns[ind].alive)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);
        if (pawns[ind].color && i > 0)
        {
            if (gauche && j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j - 1);
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j + 1);
            }
            else if (j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j - 1);
            }
            else if (j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j + 1);
            }
            // Sinon aucun recule vers l'arrière n'est possible
        }
        else if (i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j - 1);
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j + 1);
            }
            if (j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j - 1);
            }
            else if (j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j + 1);
            }
            // Sinon aucun recule vers l'arrière n'est possible
        }

        return IND_CHANGE_ALLOWED;
    }
    // printf("pawn alive %d or ind = %d", pawns[ind].alive, ind);
    return IND_PB;
}

void moveBack(Game *g, bool gauche) {
    g->ind_move = moveBackPmetre(g->allPawns[g->is_white], g->damier, g->ind_move, gauche);
}