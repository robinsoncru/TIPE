#include "rafle_eating.h"

//l'index du dernier enfant non vide croise est mis dans dirCode
int childCount(PathTree* t, Uint8* dirCode){
    int di, dj, res = 0;
    for (int k = 0; k < 4; k++) {
        getDirsFromCode(k, &di, &dj);
        if (pathTreeChild(t, dj, di) != emptyTree) {
            res++;
            *dirCode = k;
        }
    }
    return res;
}

void pawnEat(Game* g, int indEater, bool is_white, Uint8 dirCode){
    int di, dj;
    getDirsFromCode(dirCode, &di, &dj);
    killPawn(g->allPawns[is_white], g->allPawns[!is_white],
        g->damier, g->allPawns[is_white][indEater].lig + di, g->allPawns[is_white][indEater].col + dj);
    change_pawn_place(g->allPawns[is_white], g->damier, indEater,
        g->allPawns[is_white][indEater].lig + 2*di, g->allPawns[is_white][indEater].col + 2*dj);
}

void eatRafle(Game* g, int indEater, bool is_white, PathTree* t, Path* r){    
    PathTree* workTree = t;
    Uint8 dirCode;
    int nbChilds = childCount(workTree, &dirCode);
    int di, dj;
    while (nbChilds >= 1) {
        if (nbChilds >= 2) { //s'il y a plusieurs enfants
            dirCode = pathPopFirstIn(r);
        }
        pawnEat(g, indEater, is_white, dirCode);
        getDirsFromCode(dirCode, &di, &dj);
        workTree = pathTreeChild(t, dj, di);
        nbChilds = childCount(workTree, &dirCode);
    }
}