#include "rafle_counting.h"
#include "../../../../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>

void countRaflesSpitOut(Game *g, bool eatingColor, int indEater, int iEater, int jEater, int indVictim, int add0, int add1)
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

int countRaflesDivideAndGather(Game *g, bool eatingColor, int indEater, int i, int j);

int countRaflesAux(Game *g, bool eatingColor, int indEater, int add0, int add1)
{
    int oldI = get_pawn_value(g, eatingColor, indEater, LIG);
    int oldJ = get_pawn_value(g, eatingColor, indEater, COL);

    int indVictim = nonLoggingChangeForEat(g, eatingColor, indEater, oldI, oldJ, add0, add1);

    // on met a jour i et j apres avoir mange le pion ennemi
    int i = get_pawn_value(g, eatingColor, indEater, LIG);
    int j = get_pawn_value(g, eatingColor, indEater, COL);

    int res = countRaflesDivideAndGather(g, eatingColor, indEater, i, j);

    countRaflesSpitOut(g, eatingColor, indEater, i, j, indVictim, add0, add1);

    return res;
}

int countRafles(Game *g, int indSerialKiller, Coord temporaryPos)
{
    int i = get_pawn_value(g, g->is_white, indSerialKiller, LIG);
    int j = get_pawn_value(g, g->is_white, indSerialKiller, COL);
    change_pawn_place(g, indSerialKiller, g->is_white,
        temporaryPos.i, temporaryPos.j);

    //le cas particulier du pion qui ne peut pas manger doit etre
    //traite a part
    int di, dj;
    bool canEatFromPos = false;
    for (Uint8 k = 0; k < 4; k++)
    {
        // Petite astuce pour parcourir les voisins :)
        getDirsFromCode(k, &di, &dj);
        if (!eatingIsOutOfBounds(temporaryPos.i, temporaryPos.j, di, dj)
            && canEat(g, g->is_white, indSerialKiller,
                temporaryPos.i, temporaryPos.j, di, dj))
        {
            canEatFromPos = true;
            break;
        }
    }
    if (!canEatFromPos) {
        change_pawn_place(g, indSerialKiller, g->is_white, i, j);
        return 0;
    }

    int res =  countRaflesDivideAndGather(g, g->is_white, indSerialKiller,
        temporaryPos.i, temporaryPos.j);

    change_pawn_place(g, indSerialKiller, g->is_white, i, j);
    return res;
}

int countRaflesDivideAndGather(Game *g, bool eatingColor, int indEater, int i, int j)
{
    /*
    Pour un pion donne avec ses coordonnees,
    renvoie le nombre de rafles possibles
    en effectuant les appels reccurssifs de countRaflesAux*/
    int di, dj;
    int res = 0;
    for (Uint8 k = 0; k < 4; k++)
    {
        // Petite astuce pour parcourir les voisins :)
        getDirsFromCode(k, &di, &dj);
        if (!eatingIsOutOfBounds(i, j, di, dj)
            && canEat(g, eatingColor, indEater, i, j, di, dj))
        {
            res += countRaflesAux(g, eatingColor, indEater, di, dj);
        }
    }
    if (res == 0) {
        return 1;
    }

    return res;
}