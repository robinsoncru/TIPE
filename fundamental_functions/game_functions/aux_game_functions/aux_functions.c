#include "aux_functions.h"
#include <stdio.h>

/*







   AUX FUNCTIONS






*/

void copy_remove_pawn_from_index_to_index(Game *g, int indStart, int indArrive, bool color)
{
    g->damier[get_pawn_value(g, color, indStart, LIG)][get_pawn_value(g, color, indStart, COL)].ind_pawn = VOID_INDEX;

    for (int k = 1; k < 9; k++)
    {
        put_pawn_value(g, color, indArrive, k, get_pawn_value(g, color, indStart, k));
    }
    pawn_default_value_new(g, indStart, color);

    // Sur le damier
    g->damier[get_pawn_value(g, color, indArrive, LIG)][get_pawn_value(g, color, indArrive, COL)].ind_pawn = indArrive;
    g->damier[get_pawn_value(g, color, indArrive, LIG)][get_pawn_value(g, color, indArrive, COL)].pawn_color = color;
}

// void pawn_default_value(pawn pawns[], int ind, bool color)
// {
//     /* Initialize pawn with default values, identify by its index and color
//     Celle ci sera prochainement supprimee */
//     pawns[ind].alive = false;
//     pawns[ind].col = -1;
//     pawns[ind].lig = -1;
//     pawns[ind].queen = false;
//     pawns[ind].color = color;
//     pawns[ind].friendly = -1;
//     pawns[ind].ennemy = -1;
//     pawns[ind].pba = 1;
// }

void pawn_default_value_new(Game *g, int ind, bool color)
{
    /* Initialize pawn with default values, identify by its index and color */
    g->allPawns[color][ind].alive = false;
    g->allPawns[color][ind].col = -1;
    g->allPawns[color][ind].lig = -1;
    g->allPawns[color][ind].queen = false;
    g->allPawns[color][ind].color = color;
    g->allPawns[color][ind].friendly = -1;
    g->allPawns[color][ind].ennemy = -1;
    g->allPawns[color][ind].pba = 1;
}

// We are sure about the pawn we delete (no check control so be careful)
// Kills the indicated pawn and applies all the necessary rules
// Les ghosts pawn ne peuvent pas se manger entre eux: attention dans les fonctions eatPawn. Et un pion plein detruit le nuage avant de
// manger un ghost pawn
void killPawn(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j)
{
    printf("i, j = %d, %d\n", i, j);
    assert(i != -1 && j != -1);
    if (!freeCase(damier[i][j]))
    {
        int indPawn = damier[i][j].ind_pawn;
        bool color = damier[i][j].pawn_color;
        int indEnnemy = get_pawn_value(g, color, indPawn, ENNEMY);
        int indAmi = get_pawn_value(g, color, indPawn, FRIENDLY);
        if (indEnnemy != -1)
        {
            put_pawn_value(g, !color, indEnnemy, ENNEMY, -1);
            put_pawn_value(g, !color, indEnnemy, QUEEN, 1);
        }
        if (indAmi != -1)
        {
            put_pawn_value(g, !color, indAmi, FRIENDLY, -1);
        }
        pawn_default_value_new(g, indPawn, color);
        damier[i][j].ind_pawn = -1;
        /* Supprimer l'indice et optimiser la memoire */
        copy_remove_pawn_from_index_to_index(g, g->nb_pawns[color] - 1, indPawn, color);
        g->nb_pawns[color]--;
    }
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
void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col)
{
    damier[pawns[ind].lig][pawns[ind].col].ind_pawn = -1;
    pawns[ind].lig = lig;
    pawns[ind].col = col;
    damier[lig][col].ind_pawn = ind;
    damier[lig][col].pawn_color = pawns[ind].color;
}

void change_pawn_place_new(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool color, int lig, int col)
{
    damier[get_pawn_value(g, color, ind, LIG)][get_pawn_value(g, color, ind, COL)].ind_pawn = -1;
    put_pawn_value(g, color, ind, LIG, lig);
    put_pawn_value(g, color, ind, COL, col);
    damier[lig][col].ind_pawn = ind;
    damier[lig][col].pawn_color = color;
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
int nonLoggingChangeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int i, int j, int add0, int add1)
{

    int indVictim = damier[i + add0][j + add1].ind_pawn;
    // assert(ind > -1);
    change_pawn_place(pawns, damier, indEater, i + 2 * add0, j + 2 * add1);

    // Kill pawn indVictim
    NPawns[indVictim].alive = false;
    damier[i + add0][j + add1].ind_pawn = VOID_INDEX;
    // printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return indVictim;
}

// promotes the indicated pawn, un ghost pawn ne peut pas devenir une dame
void promote(Game *g, bool is_white, int ind)
{
    pawn *p = &(g->allPawns[is_white][ind]);
    p->queen = true;
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
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
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

void stormBreaks(Game *g, bool color, int indSurvivor)
{
    maillon *l = g->cloud[color];
    int iSurvivor = get_pawn_value(g, color, indSurvivor, LIG);
    int jSurvivor = get_pawn_value(g, color, indSurvivor, COL);

    put_pawn_value(g, color, indSurvivor, PBA, 1);
    while (!is_empty(l))
    {
        int ind = pop(l);
        if (ind != g->damier[iSurvivor][jSurvivor].ind_pawn)
            ;
        {
            killPawn(g, g->damier, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL));
        }
    }
    g->lengthCloud[color] = 0;

    // C'est une fonction mutuellement recursive car le pion foudroyer peut etre pres du nuage de la couleur opposee. On verifie donc
    // dans l'autre nuage

    if (canStormBreaksForTheOthers(g, g->damier[iSurvivor][jSurvivor].ind_pawn, color))
        AleatStormBreaks(g, !color);
}

void AleatStormBreaks(Game *g, bool color)
{
    int nbSurvivor = rand() % g->lengthCloud[color];
    maillon *l = g->cloud[color];
    int incr = 0;
    int iSurvivor;
    int jSurvivor;

    while (!is_empty(l))
    {
        int ind = pop(l);
        if (incr == nbSurvivor)
        {
            put_pawn_value(g, color, ind, PBA, 1);
            iSurvivor = get_pawn_value(g, color, ind, LIG);
            jSurvivor = get_pawn_value(g, color, ind, COL);
        }
        else
        {
            killPawn(g, g->damier, get_pawn_value(g, color, ind, LIG), get_pawn_value(g, color, ind, COL));
        }
        incr++;
    }
    g->lengthCloud[color] = 0;

    // C'est une fonction recursive car le pion foudroyer peut etre pres du nuage de la couleur opposee. On verifie donc
    // dans l'autre nuage

    printf("%d %d %d", iSurvivor, jSurvivor, g->damier[iSurvivor][jSurvivor].ind_pawn);
    fflush(stdout);
    if (canStormBreaksForTheOthers(g, g->damier[iSurvivor][jSurvivor].ind_pawn, color))
        AleatStormBreaks(g, !color);
}
