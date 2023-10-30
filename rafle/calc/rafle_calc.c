#include "rafle_calc.h"

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

void bestRafleAux(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int add0, int add1, Rafle* res, int length){
    int oldI = pawns[indEater].lig;
    int oldJ = pawns[indEater].col;

    int indVictim = nonLoggingChangeForEat(pawns,Npawns, damier, indEater, oldI, oldJ, add0, add1);

    //on met a jour i et j apres avoir mange le pion ennemi
    int i = pawns[indEater].lig;
    int j = pawns[indEater].col;

    if (!isEmpty(res) && length >= lengthRafle(res)) {
        emptyRafle(res);
    }

    int deltaI, deltaJ; 
    for (int k = 0; k < 4; k++) {
        //Petite astuce pour parcourir les voisins :)
        deltaI = direction(k%2);
        deltaJ = direction((k >> 1)%2);

        if (canEat(pawns, damier, indEater, i, j, deltaI, deltaJ) && !eatingIsOutOfBounds(i, j, deltaI, deltaJ)) {
            bestRafleAux(pawns, Npawns, damier, indEater, deltaI, deltaJ, res, length + 1);
        }
    }
    /*A partir de ce point, res respecte la contrainte suivante :
        -si on est a la fin d'un chemin plus long que la meilleure rafle calculee jusqu'alors, res est vide
        -sinon, res contient la completion de la meilleure rafle calculee jusqu'alors a partir d'un quelconque predecesseur
        de la case actuelle, qui peut tres bien etre elle-même.*/

    if (isEmpty(res)) {
        changeLengthOfRafle(res, length);
        addWithoutIncrRafle(res, i, j);
    }

    coord topOfRafle = peekRafle(res);
    if (topOfRafle.i == i && topOfRafle.j == j) {
        addWithoutIncrRafle(res, oldI, oldJ);
    }

    spitOut(pawns, Npawns, damier, indEater, i, j, indVictim, add0, add1);
}

Rafle* bestRafle(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indSerialKiller){
    //Entree : deux tableaux de pions, un damier, l'index du pion qui commet la rafle
    //Sortie : meilleure rafle faisable a partir de la position dudit pion
    //l'algorithme de calcul est reccursif, et la fonction auxiliaire a besoin de la donnee d'une direction
    //dans laquelle manger pour fonctionner.
    Rafle* res = createRafle();
    int i = pawns[indSerialKiller].lig;
    int j = pawns[indSerialKiller].col;

    int deltaI, deltaJ; 
    for (int k = 0; k < 4; k++) {
        //Petite astuce pour parcourir les voisins :)
        deltaI = direction(k%2);
        deltaJ = direction((k >> 1)%2);

        if (canEat(pawns, damier, indSerialKiller, i, j, deltaI, deltaJ)) {
            bestRafleAux(pawns, Npawns, damier, indSerialKiller, deltaI, deltaJ, res, 1);
        }
    }
    return res; //bah putain ! c'etait pas trivial ! mais je suis fier de moi ! Tu peux (Victor G)
}

void printBestRafle(pawn pawns[],pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indSerialKiller){
    printf("\nrafle departure : (%d, %d)\n", pawns[indSerialKiller].lig, pawns[indSerialKiller].col);
    Rafle* r = bestRafle(pawns, Npawns, damier, indSerialKiller);
    printRafle(r);
    printf("\n");
}