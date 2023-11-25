#include "debug_functions.h"

/*







   Debug functions






*/

void error()
{
    printf("check\n");
    fflush(stdout);
}

void print_pawn(pawn p, int ind)
{
    printf("Ind %d Coord %d %d Ennemy %d, Friend %d Color %d Alive %d Pba %d\n", ind, p.lig, p.col, p.ennemy, p.friendly, p.color, p.alive, p.pba);
    fflush(stdout);
}

void print_pawns(Game *g)
{
    for (int i = 0; i < 2 * NB_PAWNS; i++)
    {
        print_pawn(g->allPawns[g->is_white][i], i);
    }
}

void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG], Game *g)
{
    int ind;
    bool c;
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            ind = damier[i][j].ind_pawn;
            c = damier[i][j].pawn_color;
            if (ind != -1)
                printf("Case ligne %d col %d Coord pion %d %d\n", i, j, g->allPawns[c][ind].lig, g->allPawns[c][ind].col);
        }
    }
}

void assertAndLog(bool condition, char *message)
{
    if (!condition)
    {
        printf("\n");
        printf("Erreur : %s", message);
        printf("\n");
        assert(false);
    }
}

void flush()
{
    fflush(stdout);
}

void print_little_linked_list(maillon *l)
{
    maillon *actu = l;
    while (!is_empty(actu))
    {
        actu = actu->next;
        printf("indice %d\n", actu->ind);
    }
}