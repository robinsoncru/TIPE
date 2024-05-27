#include "rafle_tree_calc.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>

void spitOut(Game *g, bool eatingColor, int indEater, int iEater, int jEater, int indVictim, int add0, int add1)
{
    // fonction reciproque de changeForEat
    // pour un pion d'index indEater aux coordonnees (iEater, jEater), annule son action de manger indVictim
    // dans la direction add0 et add1

    put_case_damier(g, iEater - add0, jEater - add1, PAWN_COLOR, bool_to_int(!eatingColor));
    put_case_damier(g, iEater - add0, jEater - add1, IND_PAWN_ON_CASE, indVictim);
    put_case_damier(g, iEater, jEater, IND_PAWN_ON_CASE, VOID_INDEX);
    put_case_damier(g, iEater - 2 * add0, jEater - 2 * add1, PAWN_COLOR, eatingColor);
    put_case_damier(g, iEater - 2 * add0, jEater - 2 * add1, IND_PAWN_ON_CASE, indEater);

    put_pawn_value(g, !eatingColor, indVictim, ALIVE, 1);
    put_pawn_value(g, !eatingColor, indVictim, LIG, iEater - add0);
    put_pawn_value(g, !eatingColor, indVictim, COL, jEater - add1);
    put_pawn_value(g, eatingColor, indEater, LIG, iEater - 2 * add0);
    put_pawn_value(g, eatingColor, indEater, COL, jEater - 2 * add1);
}

PathTree *divideAndGather(Game *g, bool eatingColor, int indEater, int i, int j);

PathTree *rafleTreeCalcAux(Game *g, bool eatingColor, int indEater, int add0, int add1)
{
    int oldI = get_pawn_value(g, eatingColor, indEater, LIG);
    int oldJ = get_pawn_value(g, eatingColor, indEater, COL);

    int indVictim = nonLoggingChangeForEat(g, eatingColor, indEater, oldI, oldJ, add0, add1);

    // on met a jour i et j apres avoir mange le pion ennemi
    int i = get_pawn_value(g, eatingColor, indEater, LIG);
    int j = get_pawn_value(g, eatingColor, indEater, COL);

    PathTree *res = divideAndGather(g, eatingColor, indEater, i, j);

    spitOut(g, eatingColor, indEater, i, j, indVictim, add0, add1);

    return res;
}

PathTree *rafleTreeCalc(Game *g, bool eatingColor, int indSerialKiller)
{
    int i = get_pawn_value(g, eatingColor, indSerialKiller, LIG);
    int j = get_pawn_value(g, eatingColor, indSerialKiller, COL);

    PathTree *res = divideAndGather(g, eatingColor, indSerialKiller, i, j);

    return res;
}

PathTree *divideAndGather(Game *g, bool eatingColor, int indEater, int i, int j)
{
    /*
    Pour un pion donne avec ses coordonnees,
    renvoie son arbre de rafles a partir de la case indiquee en effectuant les
    appels reccursifs de rafleTreeCalcAux*/
    int di, dj;
    PathTree *res = pathTreeCreateNode(i, j);
    PathTree *child;
    for (Uint8 k = 0; k < 4; k++)
    {
        // Petite astuce pour parcourir les voisins :)
        getDirsFromCode(k, &di, &dj);
        if (!eatingIsOutOfBounds(i, j, di, dj)
            && canEat(g, eatingColor, indEater, i, j, di, dj))
        {
            child = rafleTreeCalcAux(g, eatingColor, indEater, di, dj);
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