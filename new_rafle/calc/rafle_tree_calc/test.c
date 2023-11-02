#include "test.h"
#include <stdbool.h>
#include <stdio.h>

PathTree* firstChildEncountered(PathTree* t){
    int di, dj;
    PathTree* child;
    for (int k = 0; k < 4; k++) {
        getDirsFromCode(k, &di, &dj);

        child = pathTreeChild(t, dj, di);
        if (child != emptyTree) {
            return child;
        }
    }
    return emptyTree;
}

void eatingTest(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], PathTree* pathTree){
    if (pathTree != emptyTree) {
        Coord initPos = pathTreeLabel(pathTree);
        int indEater = damier[initPos.i][initPos.j].ind_pawn;
        Coord vector, InitPos, FinalPos;
        PathTree* parent = pathTree;
        PathTree* child = firstChildEncountered(pathTree);
        while (child != emptyTree) {
            
        }
    }
}

int testRafleTree(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind){
    printf("\ntestRafleTree called\nind = %d\n", ind);
    if (ind == NEUTRAL_IND) {
        return NEUTRAL_IND;
    }
    PathTree* pathTree = rafleTreeCalc(pawns, NPawns, damier, ind);
    printf("\npathTree generated.\n");
    if (pathTree == emptyTree) {
        return IND_PB;
    }
    printf("\npresence of possible paths : %s\n", (pathTreeDepth(pathTree) > 0) ? "true" : "false");
    if (pathTreeDepth(pathTree) == 0) {
        return ind;
    }
    eatingTest(pawns, NPawns, damier, pathTree);
    return IND_CHANGE_ALLOWED;
}