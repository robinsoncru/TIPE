#include "calc_proba_trivial.h"

double esperancefLignesPions(Game *g, bool color, double (*f)(int))
{
    // Renvoie la moyenne des f(i) où i est la ligne de chacun des pions
    double esp = 0.0;
    double n = g->nb_pawns[color];
    double p = 1.0 / n;
    for (int ind = 0; ind < n; ind++)
    {
        double val = f(get_pawn_value(g, color, ind, LIG));
        esp += val * p;
    }
    return esp;
}

double carre(int i)
{
    return i * i;
}

double idIntTodouble(int i)
{
    return i;
}

double esperanceLignesPions(Game *g, bool color)
{
    return esperancefLignesPions(g, color, idIntTodouble);
}

double varianceLignesPions(Game *g, bool color)
{
    // V(X) = E(X)**2 - E(X**2);
    double esp = esperanceLignesPions(g, color);
    return esp * esp - esperancefLignesPions(g, color, carre);
}