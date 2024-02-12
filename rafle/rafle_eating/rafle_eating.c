#include "rafle_eating.h"
#include <stdint.h>
#include <stdio.h>

void pawnEat(Game* g, int indEater, bool is_white, int di, int dj){
    pawn* p = &(g->allPawns[is_white][indEater]);
    killPawn(g, g->damier, p->lig + di, p->col + dj);
    change_pawn_place(g->allPawns[is_white], g->damier, indEater,
        p->lig + 2*di, p->col + 2*dj);
}

void eatRafle(Game* g, int indEater, bool is_white, PathTree* t, Path* r){    
    PathTree* workTree = t;
    uint8_t dirCode;
    int di, dj;

    while (pathTreeDepth(workTree) > 0) {
        switch (pathTreeNBChilds(workTree)) {
            case 1:
                dirCode = pathTreeFirstChild(workTree);
                break;
            default:
                dirCode = pathPopFirstIn(r);
                break;
        }
        getDirsFromCode(dirCode, &di, &dj);
        pawnEat(g, indEater, is_white, di, dj);
        workTree = pathTreeChild(workTree, dj, di);
    }
    endTurnGameManagement(g, g->is_white, g->ind_move, IND_CHANGE_ALLOWED, false);
}

bool eatRafleNGM(Game* g, int indEater, bool is_white, PathTree* t, Path* r){    
    // eatRafleNoGameManagement
    PathTree* workTree = t;
    uint8_t dirCode;
    int di, dj;
    bool had_eaten = false;

    while (pathTreeDepth(workTree) > 0) {
        switch (pathTreeNBChilds(workTree)) {
            case 1:
                dirCode = pathTreeFirstChild(workTree);
                break;
            default:
                dirCode = pathPopFirstIn(r);
                break;
        }
        getDirsFromCode(dirCode, &di, &dj);
        pawnEat(g, indEater, is_white, di, dj);
        workTree = pathTreeChild(workTree, dj, di);
        if (!had_eaten) had_eaten = true;
    }
    return had_eaten;
}

Path* lazyRafle(PathTree* t){
    Path* res = pathCreate(pathTreeDepth(t));
    PathTree* workTree = t;
    uint8_t dirCode;
    int di, dj;
    while (pathTreeNBChilds(workTree) > 0) {
        dirCode = pathTreeFirstChild(workTree);
        if (pathTreeNBChilds(workTree) > 1) {
            pathAdd(dirCode, res);
        }
        getDirsFromCode(dirCode, &di, &dj);
        workTree = pathTreeChild(workTree, dj, di);
    }
    return res;
}