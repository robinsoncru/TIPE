#include "rafle_tree_calc.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>

void spitOut(Game *g, bool color, int indEater, int iEater, int jEater, int indVictim, int add0, int add1)
{
    // fonction reciproque de changeForEat
    // pour un pion d'index indEater aux coordonnees (iEater, jEater), annule son action de manger indVictim
    // dans la direction add0 et add1

    put_case_damier(g, iEater - add0, jEater - add1, PAWN_COLOR, bool_to_int(!color));
    put_case_damier(g, iEater - add0, jEater - add1, IND_PAWN_ON_CASE, indVictim);
    put_case_damier(g, iEater, jEater, IND_PAWN_ON_CASE, VOID_INDEX);
    put_case_damier(g, iEater - 2 * add0, jEater - 2 * add1, PAWN_COLOR, color);
    put_case_damier(g, iEater - 2 * add0, jEater - 2 * add1, IND_PAWN_ON_CASE, indEater);

    put_pawn_value(g, !color, indVictim, ALIVE, 1);
    put_pawn_value(g, !color, indVictim, LIG, iEater - add0);
    put_pawn_value(g, !color, indVictim, COL, jEater - add1);
    put_pawn_value(g, color, indEater, LIG, iEater - 2 * add0);
    put_pawn_value(g, color, indEater, COL, jEater - 2 * add1);
}

PathTree *divideAndGather(Game *g, bool color, int indEater, int i, int j);

PathTree *rafleTreeCalcAux(Game *g, bool color, int indEater, int add0, int add1)
{
    int oldI = get_pawn_value(g, color, indEater, LIG);
    int oldJ = get_pawn_value(g, color, indEater, COL);

    int indVictim = nonLoggingChangeForEat(g, color, indEater, oldI, oldJ, add0, add1);

    // on met a jour i et j apres avoir mange le pion ennemi
    int i = get_pawn_value(g, color, indEater, LIG);
    int j = get_pawn_value(g, color, indEater, COL);

    PathTree *res = divideAndGather(g, color, indEater, i, j);

    spitOut(g, color, indEater, i, j, indVictim, add0, add1);

    return res;
}

PathTree *rafleTreeCalc(Game *g, bool color, int indSerialKiller)
{
    int i = get_pawn_value(g, color, indSerialKiller, LIG);
    int j = get_pawn_value(g, color, indSerialKiller, COL);

    PathTree *res = divideAndGather(g, color, indSerialKiller, i, j);

    return res;
}

PathTree *divideAndGather(Game *g, bool color, int indEater, int i, int j)
{
    /*
    Pour un pion donne avec ses coordonnees, renvoie son arbre de rafles a partir de la case indiquee en effectuant les
    appels reccursifs de rafleTreeCalcAux*/
    int di, dj;
    PathTree *res = pathTreeCreateNode(i, j);
    PathTree *child;
    for (Uint8 k = 0; k < 4; k++)
    {
        // Petite astuce pour parcourir les voisins :)
        getDirsFromCode(k, &di, &dj);
        if (!eatingIsOutOfBounds(i, j, di, dj) && canEat(g, color, indEater, i, j, di, dj))
        {
            child = rafleTreeCalcAux(g, color, indEater, di, dj);
            pathTreeConnect(res, child, dj, di);
        }
    }

    // On suprime les enfants inutiles.
    // C'est de l'eugenisme, donc ca reste dans le theme des rafles.
    int maxDepth = pathTreeDepth(res) - 1;
    int depth;
    for (Uint8 k = 0; k < 4; k++)
    {
        getDirsFromCode(k, &di, &dj);

        PathTree *inspected = pathTreeChild(res, dj, di);
        depth = pathTreeDepth(inspected);
        if (inspected != emptyTree && depth < maxDepth)
        {
            pathTreeEmptyChild(res, dj, di);
        }
    }

    return res;
}