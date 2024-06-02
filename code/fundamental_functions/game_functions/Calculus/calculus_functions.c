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
    return integer == 1;
}

int bool_to_int(bool b)
{
    if (b){
        return 1;
    }
    return 0;
}

int ind_from_lig_col(Game *g, int lig, int col) {
    // Give the ind from the coord in the damier
    assert(inGame(lig, col));
    return g->damier[lig][col].ind_pawn;
}

int ind_from_coord(Game *g, Coord pos) {
    // Give the ind from the coord in the damier
    return ind_from_lig_col(g, pos.i, pos.j);
}

Coord coord_from_ind(Game *g, int ind, bool color) {
    // Donne les coordonnees du pion a partir de l'indice
    assert(isValidIndexInGame(g, ind, color));
    Coord coord = {.i = g->allPawns[color][ind].lig, .j = g->allPawns[color][ind].col};
    return coord;
}

int quickPow(int a, int n)
{
    /* Return a^n, exp rapide */
    if (n == 0)
        return 1;
    else
    {
        int b = quickPow(a, n / 2);
        if (n % 2 == 1)
            return a * b * b;
        else
            return b * b;
    }
}