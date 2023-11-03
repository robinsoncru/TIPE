#include "test_tree_display.h"
#include <stdio.h>

int pathTreeDisplayTest(SDL_Renderer* render, pawn* pawns, pawn* NPawns, Case damier[NB_CASE_LG][NB_CASE_LG], int ind){
    if (ind == NEUTRAL_IND) {
        return IND_PB;
    }
    printf("\npathTreeDisplayTest called\n");
    PathTree* t = rafleTreeCalc(pawns, NPawns, damier, ind);
    printf("\nrafle tree generated\n");
    pathTreeDisplay(render, damier, t);
    printf("\nrafle tree displayed\n");
    pathTreeFree(t);
    return ind;
}