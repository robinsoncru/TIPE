#include "heuristiques.h"

float heuristique_miam_trivial(Game *g)
{
    // Favorise d'avoir plus de pion, temps constant
    bool color = g->is_white;
    int nb_color_actu = g->nb_pawns[color];
    int nb_color_oppose = g->nb_pawns[!color];
    int nb_dame_color_actu = g->nbQueenWithFriend[color] + g->nbQueenWithoutFriend[color];
    int nb_dame_color_opp = g->nbQueenWithFriend[!color] + g->nbQueenWithoutFriend[!color];
    int favo = 4; // Une dame vaut 5 pion car on recompte les dames dans nb_color_...
    if (nb_color_actu == 0 && nb_color_oppose == 0)
    {
        return 0;
    }
    else if (nb_color_actu == 0)
    {
        return -INFINITY;
    }
    else if (nb_color_oppose == 0)
    {
        return INFINITY;
    }
    else
    {
        return (favo * nb_dame_color_actu + nb_color_actu - (favo * nb_dame_color_opp + nb_color_oppose));
    }
}

float heuristique_border(Game *g)
{
    // Favorise les pions proches de la ligne ennemie
    return 0;
}