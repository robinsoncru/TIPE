#include "calc_proba.h"

float esperancefLignesPions(Game *g, bool color, double (*f)(int))
{
    // Renvoie la moyenne des f(i) où i est la ligne de chacun des pions
    int esp = 0.0;
    float n = g->nb_pawns[color];
    float p = 1.0 / n;
    for (int ind = 0; ind < n; ind++)
    {
        esp += f(get_pawn_value(g, color, ind, LIG)) * p;
    }
    return esp;
}

float carre(int i)
{
    return i * i;
}

float idIntToFloat(int i)
{
    return i;
}

float esperanceLignesPions(Game *g, bool color)
{
    return esperancefLignesPions(g, color, idIntToFloat);
}

float varianceLignesPions(Game *g, bool color)
{
    // V(X) = E(X)**2 - E(X**2);
    float esp = esperanceLignesPions(g, color);
    return esp * esp - esperancefLignesPions(g, color, carre);
}