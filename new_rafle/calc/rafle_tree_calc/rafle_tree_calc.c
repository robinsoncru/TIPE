#include "rafle_tree_calc.h"

void spitOut(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int iEater, int jEater, int indVictim, int add0, int add1){
    //fonction reciproque de changeForEat
    //pour un pion d'index indEater aux coordonnees (iEater, jEater), annule son action de manger indVictim
    //dans la direction add0 et add1

    damier[iEater - add0][jEater - add1].pawn_color = !pawns[indEater].color;
    damier[iEater - add0][jEater - add1].ind_pawn = indVictim;
    damier[iEater][jEater].ind_pawn = VOID_INDEX;
    damier[iEater - 2 * add0][jEater - 2 * add1].pawn_color = pawns[indEater].color;
    damier[iEater - 2 * add0][jEater - 2 * add1].ind_pawn = indEater;

    Npawns[indVictim].alive = true;
    Npawns[indVictim].lig = iEater - add0;
    Npawns[indVictim].col = jEater - add1;
    pawns[indEater].lig = iEater - 2 * add0;
    pawns[indEater].col = jEater - 2 * add1;
}

int direction(int a){
    return (a == 0) ? 1 : -1;
}

bool outOfBounds(int i, int j){
    //Checks if the (i, j) position is out of bounds
    return i < 0 || i >= NB_CASE_LG || j < 0 || j >= NB_CASE_LG;
}

bool eatingIsOutOfBounds(int i, int j, int add0, int add1){
    //Checks if eating from position (i, j) in the (add0, add1) direction leads to an out of bounds position
    return outOfBounds(i + 2 * add0, j + 2 * add1);
}

PathTree* divideAndGather(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int i, int j);

PathTree* rafleTreeCalcAux(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int add0, int add1){
    int oldI = pawns[indEater].lig;
    int oldJ = pawns[indEater].col;

    int indVictim = nonLoggingChangeForEat(pawns,Npawns, damier, indEater, oldI, oldJ, add0, add1);

    //on met a jour i et j apres avoir mange le pion ennemi
    int i = pawns[indEater].lig;
    int j = pawns[indEater].col;

    PathTree* res = divideAndGather(pawns, Npawns, damier, indEater, i, j);

    spitOut(pawns, Npawns, damier, indEater, i, j, indVictim, add0, add1);
    
    return res;
}

PathTree* rafleTreeCalc(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indSerialKiller){

    int i = pawns[indSerialKiller].lig;
    int j = pawns[indSerialKiller].col;

    PathTree* res = divideAndGather(pawns, Npawns, damier, indSerialKiller, i, j);
    
    return res;
}

PathTree* divideAndGather(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int i, int j){
    /*
    Pour un pion donne avec ses coordonnees, renvoie son arbre de rafles a partir de la case indiquee en effectuant les
    appels reccursifs de rafleTreeCalcAux*/
    int deltaI, deltaJ;
    PathTree* res = pathTreeCreateNode(i, j);
    PathTree* child;
    for (int k = 0; k < 4; k++) {
        //Petite astuce pour parcourir les voisins :)
        deltaI = direction(k%2);
        deltaJ = direction((k >> 1)%2);

        if (canEat(pawns, damier, indEater, i, j, deltaI, deltaJ) && !eatingIsOutOfBounds(i, j, deltaI, deltaJ)) {
            child = rafleTreeCalcAux(pawns, Npawns, damier, indEater, deltaI, deltaJ);
            pathTreeConnect(res, child, deltaI, deltaJ);
        }
    }

    //On suprime les enfants inutiles.
    //C'est de l'eugenisme, donc ca reste dans le theme des rafles.
    int maxDepth = pathTreeDepth(res) - 1;
    for (int k = 0; k < 4; k++) {
        deltaI = direction(k%2);
        deltaJ = direction((k >> 1)%2);

        PathTree* inspected = pathTreeChild(res, deltaI, deltaJ);
        if (pathTreeDepth(inspected) < maxDepth) {
            pathTreeFree(inspected);
        }
    }

    return res;
}