#include "calculus_functions.h"

/*







   CALCULUS FUNCTIONS






*/

int NON(int b)
{
    return (b + 1) % 2;
}

int abs(int a)
{
    return (a >= 0) ? a : -a;
}

// la direction est donnee sous la forme d'un couple
//(dx, dy) ou dx et dy peuvent valoir -1 ou 1.
// On lui associe un nombre entre 0 et 3 inclus
// dont le bit de poids faible est la direction horizontale
// et le bit de poids fort la direction verticale.
// 0 sens negatif, 1 sens positif

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

// handle structure

void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value)
{
    /* 1:alive | 2:ennemy | 3:friendly | 4:queen | 5:lig | 6:col | 7:pba | 8:color
    Permet de modifier les pmetres d'un pion de la liste de g reperer avec sa couleur, wich_pmetre_modify modifie le i-ième pmetre */
    switch (wich_pmetre_modify)
    {
    case 1:
        g->allPawns[color][ind].alive = int_to_bool(value);
        break;
    case 2:
        g->allPawns[color][ind].ennemy = value;
        break;
    case 3:
        g->allPawns[color][ind].friendly = value;
        break;
    case 4:
        g->allPawns[color][ind].queen = int_to_bool(value);
        break;
    case 5:
        g->allPawns[color][ind].lig = value;
        break;
    case 6:
        g->allPawns[color][ind].col = value;
        break;
    case 7:
        g->allPawns[color][ind].pba = value;
        break;
    case 8:
        g->allPawns[color][ind].color = int_to_bool(value);
        break;
    default:
        // Do nothing
        printv("put pawn value error");
        break;
    }
}

int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get)
{
    pawn *p = &(g->allPawns[color][ind]); // error

    /* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
    switch (wich_pmetre_get)
    {
    case 1:
        return bool_to_int(p->alive);
    case 2:
        return p->ennemy;
    case 3:
        return p->friendly;
    case 4:
        return bool_to_int(p->queen);
    case 5:
        return p->lig;
    case 6:
        return p->col;
    case 7:
        return p->pba;
    case 8:
        return p->color;
    default:
        // Do nothing
        printv("get pawn value error");
        return VOID_INDEX;
    }
}

int ind_from_coord(Game *g, int lig, int col) {
    // Give the ind from the coord in the damier
    return g->damier[lig][col].ind_pawn;
}