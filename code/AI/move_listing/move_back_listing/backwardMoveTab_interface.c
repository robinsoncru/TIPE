#include "backwardMoveTab_interface.h"
#include "backwardMoveTab_struct.h"
#include "../../../fundamental_functions/game_functions/access_functions/access_functions.h"
#include <stdio.h>
#include <stdlib.h>

backwardMoveTab_t* backwardMoveTabCreate(int c){
    backwardMoveTab_t* res = malloc(sizeof(backwardMoveTab_t));
    res->n = c;
    res->tab = malloc(c * sizeof(backwardMove_t));
    return res;
}

backwardMoveTab_t* backwardMoveTabCopy(backwardMoveTab_t* t){
    int n = t->n;
    backwardMoveTab_t* res = backwardMoveTabCreate(n);
    for (int i = 0; i < n; i++) {
        res->tab[i] = t->tab[i];
    }
    return res;
}

void backwardMoveTabSetIndMovedPawn(backwardMoveTab_t* t, int i, int newInd){
    t->tab[i].indMovedPawn = newInd;
}
void backwardMoveTabSetDir(backwardMoveTab_t* t, int i, int newDir){
    t->tab[i].dir = newDir;
}
int backwardMoveTabGetIndMovedPawn(backwardMoveTab_t* t, int i){
    return t->tab[i].indMovedPawn;
}
int backwardMoveTabGetDir(backwardMoveTab_t* t, int i){
    return t->tab[i].dir;
}
void backwardMoveTabFree(backwardMoveTab_t* t){
    free(t->tab);
    free(t);
}

void backwardMoveTabPrint(backwardMoveTab_t* t){
    char dirTab[] = {'<', '|', '>'};
    backwardMove_t currentValue;
    printf("\n[");
    if (t->n > 0) {
        currentValue = t->tab[0];
        printf("(%c, %d)", dirTab[currentValue.dir + 1], currentValue.indMovedPawn);
    }

    for (int i = 1; i < t->n; i++) {
        currentValue = t->tab[i];
        printf(", (%c, %d)", dirTab[currentValue.dir + 1], currentValue.indMovedPawn);
    }
    printf("]\n");
    flush();
}