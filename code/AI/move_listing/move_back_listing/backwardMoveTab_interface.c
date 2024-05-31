#include "backwardMoveTab_interface.h"
#include "backwardMoveTab_struct.h"
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