#include "heuristiques.h"

double heuristique_miam_trivial(Game *g)
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

float heuristique_miam(Game *g)
{
    /* Plus précis mais en n*n ou n est le nombre de pion, de plus, la situation peut être bien évalué mais
    le chemin pour y arrivé avec le min max peut être merdique (laissé des pions en prises par exemple)
    Pour y remédier, supposons que ce soit au blanc de jouer, on attribue un gros score si les blancs
    on une opportunité de manger un seul pion noir, s'ils ont la possibilité d'en manger pls, c'est qu'ils ont
    laissés des prises aux tours d'avant, donc on attribuera un mauvais score. Pour laisser le jeu mouvoir,
    je ne récompense pas les pions d'être dans certaines regions du plateau (les bords ...), heuristique
    border s'en occupera */
    bool color = g->is_white;
    int nb_color_actu = g->nb_pawns[color];
    int nb_color_oppose = g->nb_pawns[!color];
    int nb_dame_color_actu = g->nbQueenWithFriend[color] + g->nbQueenWithoutFriend[color];
    int nb_dame_color_opp = g->nbQueenWithFriend[!color] + g->nbQueenWithoutFriend[!color];

    // Est ce qu'on peut manger ?
    int combien_fois_manger = 0;
    for (int ind = 0; ind < nb_color_actu; ind++)
    {
        int di, dj;
        int i = get_pawn_value(g, color, ind, LIG);
        int j = get_pawn_value(g, color, ind, COL);
        for (Uint8 k = 0; k < 4; k++)
        {
            // Petite astuce pour parcourir les voisins :)
            getDirsFromCode(k, &di, &dj);
            if (!eatingIsOutOfBounds(i, j, di, dj) && canEat(g, color, ind, i, j, di, dj))
            {
                
            }
        }
    }
    return 0;
}

double heuristique_border(Game *g)
{
    // Favorise les pions proches de la ligne ennemie
    return 0;
}