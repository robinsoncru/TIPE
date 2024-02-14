#include "rafle_tree_calc.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>

void spitOut(pawn *pawns, pawn *NPawns, Case **damier, int indEater, int iEater, int jEater, int indVictim, int add0, int add1){
    //fonction reciproque de changeForEat
    //pour un pion d'index indEater aux coordonnees (iEater, jEater), annule son action de manger indVictim
    //dans la direction add0 et add1

    damier[iEater - add0][jEater - add1].pawn_color = !pawns[indEater].color;
    damier[iEater - add0][jEater - add1].ind_pawn = indVictim;
    damier[iEater][jEater].ind_pawn = VOID_INDEX;
    damier[iEater - 2 * add0][jEater - 2 * add1].pawn_color = pawns[indEater].color;
    damier[iEater - 2 * add0][jEater - 2 * add1].ind_pawn = indEater;

    NPawns[indVictim].alive = true;
    NPawns[indVictim].lig = iEater - add0;
    NPawns[indVictim].col = jEater - add1;
    pawns[indEater].lig = iEater - 2 * add0;
    pawns[indEater].col = jEater - 2 * add1;
}

PathTree* divideAndGather(pawn *pawns,pawn *NPawns, Case **damier, int indEater, int i, int j);

PathTree* rafleTreeCalcAux(pawn *pawns,pawn *NPawns, Case **damier, int indEater, int add0, int add1){
    int oldI = pawns[indEater].lig;
    int oldJ = pawns[indEater].col;

    int indVictim = nonLoggingChangeForEat(pawns,NPawns, damier, indEater, oldI, oldJ, add0, add1);

    //on met a jour i et j apres avoir mange le pion ennemi
    int i = pawns[indEater].lig;
    int j = pawns[indEater].col;

    PathTree* res = divideAndGather(pawns, NPawns, damier, indEater, i, j);

    spitOut(pawns, NPawns, damier, indEater, i, j, indVictim, add0, add1);
    
    return res;
}

PathTree* rafleTreeCalc(pawn *pawns,pawn *NPawns, Case **damier, int indSerialKiller){
    int i = pawns[indSerialKiller].lig;
    int j = pawns[indSerialKiller].col;

    PathTree* res = divideAndGather(pawns, NPawns, damier, indSerialKiller, i, j);
    
    return res;
}

PathTree* divideAndGather(pawn *pawns,pawn *NPawns, Case **damier, int indEater, int i, int j){
    /*
    Pour un pion donne avec ses coordonnees, renvoie son arbre de rafles a partir de la case indiquee en effectuant les
    appels reccursifs de rafleTreeCalcAux*/
    int di, dj;
    PathTree* res = pathTreeCreateNode(i, j);
    PathTree* child;
    for (Uint8 k = 0; k < 4; k++) {
        //Petite astuce pour parcourir les voisins :)
        getDirsFromCode(k, &di, &dj);
        if (!eatingIsOutOfBounds(i, j, di, dj) && canEat(pawns, damier, indEater, i, j, di, dj)) {
            child = rafleTreeCalcAux(pawns, NPawns, damier, indEater, di, dj);
            pathTreeConnect(res, child, dj, di);
        }
    }

    //On suprime les enfants inutiles.
    //C'est de l'eugenisme, donc ca reste dans le theme des rafles.
    int maxDepth = pathTreeDepth(res) - 1;
    int depth;
    for (Uint8 k = 0; k < 4; k++) {
        getDirsFromCode(k, &di, &dj);

        PathTree* inspected = pathTreeChild(res, dj, di);
        depth = pathTreeDepth(inspected);
        if (inspected != emptyTree && depth < maxDepth) {
            pathTreeEmptyChild(res, dj, di);
        }
    }

    return res;
}