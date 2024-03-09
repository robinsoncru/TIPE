#include "test_struct_memory.h"

int NON(int b)
{
    return (b + 1) % 2;
}

bool int_to_bool(int integer)
{
    if (integer == 1)
        return true;
    else
        return false;
}

int bool_to_int(bool b)
{
    if (b)
        return 1;
    else
        return 0;
}

void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value)
{
    /* 1:alive | 2:ennemy | 3:friendly | 4:queen | 5:lig | 6:col | 7:pba | 8:color
    Permet de modifier les pmetres d'un pion de la liste de g reperer avec sa couleur, wich_pmetre_modify modifie le i-ième pmetre */
    switch (wich_pmetre_modify)
    {
    case ALIVE:
        g->allPawns[color][ind].alive = int_to_bool(value);
        break;
    case ENNEMY:
        g->allPawns[color][ind].ennemy = value;
        break;
    case FRIENDLY:
        g->allPawns[color][ind].friendly = value;
        break;
    case QUEEN:
        g->allPawns[color][ind].queen = int_to_bool(value);
        break;
    case LIG:
        g->allPawns[color][ind].lig = value;
        break;
    case COL:
        g->allPawns[color][ind].col = value;
        break;
    case PBA:
        g->allPawns[color][ind].pba = value;
        break;
    case COLOR:
        g->allPawns[color][ind].color = int_to_bool(value);
        break;
    default:
        // Do nothing
        break;
    }
}

int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get)
{
    pawn *p = &(g->allPawns[color][ind]); // error

    /* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
    switch (wich_pmetre_get)
    {
    case ALIVE:
        return bool_to_int(p->alive);
    case ENNEMY:
        return p->ennemy;
    case FRIENDLY:
        return p->friendly;
    case QUEEN:
        return bool_to_int(p->queen);
    case LIG:
        return p->lig;
    case COL:
        return p->col;
    case PBA:
        return p->pba;
    case COLOR:
        return p->color;
    default:
        // Do nothin
        return VOID_INDEX;
    }
}

void pawn_default_value_new(Game *g, int ind, bool color)
{
    /* Initialize pawn with default values, identify by its index and color */
    g->allPawns[bool_to_int(color)][ind].alive = false;
    // g->allPawns[color][ind].col = -1;
    // g->allPawns[color][ind].lig = -1;
    // g->allPawns[color][ind].queen = false;
    // g->allPawns[color][ind].color = color;
    // g->allPawns[color][ind].friendly = -1;
    // g->allPawns[color][ind].ennemy = -1;
    // g->allPawns[color][ind].pba = 1;
}

void init_pawn(Game *g, Case **damier, int i, int init_place, int add, bool init_is_white)
{
    if (init_is_white)
    {
        put_pawn_value(g, init_is_white, i, LIG, init_place);
        damier[init_place][add + 2 * i - init_place * NB_CASE_LG].pawn_color = true;
        put_pawn_value(g, init_is_white, i, COL, add + 2 * i - init_place * NB_CASE_LG);
    }
    else
    {
        put_pawn_value(g, init_is_white, i, LIG, NB_CASE_LG - init_place - 1);
        // pawns[i].lig = init_place;
        damier[NB_CASE_LG - init_place - 1][NON(add) + 2 * i - init_place * NB_CASE_LG].pawn_color = false;
        put_pawn_value(g, init_is_white, i, COL, NON(add) + 2 * i - init_place * NB_CASE_LG);
    }
    damier[get_pawn_value(g, init_is_white, i, LIG)][get_pawn_value(g, init_is_white, i, COL)].ind_pawn = i;
    put_pawn_value(g, init_is_white, i, COLOR, init_is_white);
}

void init_pawns(Game *g, bool init_is_white)
{
    int init_place = 0;
    for (int i = 0; i < NB_PAWNS; i++)
    for (int i=0; i < NB_PAWNS/3; i++)
    {
        if (init_place % 2 == 0)
        {
            if (2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(g, g->damier, i, init_place, 0, init_is_white);
            // init_pawn(pawns, g.damier, g.is_white, i, init_place+2, 0);
            else
            {
                init_place++;
                init_pawn(g, g->damier, i, init_place, 1, init_is_white);
                // init_pawn(pawns, g.damier, g.is_white, i, init_place, 1);
            }
        }
        else
        {
            if (1 + 2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(g, g->damier, i, init_place, 1, init_is_white);
            else
            {
                init_place++;
                init_pawn(g, g->damier, i, init_place, 0, init_is_white);
            }
        }
        // put_pawn_value(g, init_is_white, 0, ALIVE, true); // Here it fails
        g->allPawns[init_is_white][i].alive = true;
        // g->allPawns[init_is_white][i].queen = false;
        // g->allPawns[init_is_white][i].friendly = -1;
        // g->allPawns[init_is_white][i].ennemy = -1;
        // g->allPawns[init_is_white][i].pba = 1;
    }

    // Initialize the rest of pawns with default pmetre and the good color
    for (int i = NB_PAWNS; i < 2 * NB_PAWNS; i++)
    {
        pawn_default_value_new(g, i, init_is_white);
    }
}

Game *create_game()
{
    Game *g = malloc(sizeof(Game));
    g->damier = malloc(NB_CASE * sizeof(Case *));
    for (int i = 0; i < NB_CASE; i++)
    {
        g->damier[i] = malloc(NB_CASE * sizeof(Case));
    }
    g->allPawns = malloc(2 * sizeof(pawn *));
    g->allPawns[0] = malloc(2*NB_PAWNS * sizeof(pawn));
    g->allPawns[1] = malloc(2*NB_PAWNS * sizeof(pawn));
    init_pawns(g, true);
    init_pawns(g, false);
    return g;
}

int main(int argc, char *argv[])
{
    Game *g = create_game();
    free(g->allPawns[0]);
    free(g->allPawns[1]);
    free(g->allPawns);
    for (int i = 0; i < NB_CASE; i++)
    {
        free(g->damier[i]);
    }
    free(g->damier);
    free(g);
    return EXIT_SUCCESS;
}