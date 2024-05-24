#include "rafle_eating.h"
#include "../../fundamental_functions/game_functions/precondition/precondition.h"
#include "../../fundamental_functions/game_functions/debug/debug_functions.h"
#include <stdint.h>
#include <stdio.h>

void pawnEat(Game* g, int indEater, bool is_white, int di, int dj){
    assertAndLog(isValidIndexInGame(g, indEater, is_white),
        "invalid index in game");
    pawn p = get_pawn(g, is_white, indEater);
    assertAndLog(!eatingIsOutOfBounds(p.lig, p.col, di, dj),
        "eating is out of bounds");
    killPawn(g, p.lig + di, p.col + dj);
    change_pawn_place(g, indEater, is_white,
        p.lig + 2*di, p.col + 2*dj);
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
        had_eaten = true;
    }
    return had_eaten;
}

Path* lazyRafle(PathTree* t){
    assertAndLog(t != emptyTree, "empty tree doesn't have rafles");
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