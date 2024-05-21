#include "test.h"
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

void eatingTest(Game *g, bool color, PathTree* t){
    printf("\neatingTest called\n");
    if (t != emptyTree) {
        Coord startPos = pathTreeLabel(t);
        int indEater = get_case_damier(g, startPos.i, startPos.j).ind_pawn;
        Coord vector, initPos, finalPos;
        PathTree* parent = t;
        PathTree* child = firstChildEncountered(t);
        while (child != emptyTree) {
            initPos = pathTreeLabel(parent);
            finalPos = pathTreeLabel(child);
            vector = sub(finalPos, initPos);
            vector = reduceNormOfOne(vector);
            nonLoggingChangeForEat(g, color, indEater, initPos.i, initPos.j, vector.i, vector.j);
            parent = child;
            child = firstChildEncountered(child);
        }
    }
}

int testRafleTreePmetre(Game *g, bool color, int ind){
    if (ind == NEUTRAL_IND) {
        return NEUTRAL_IND;
    }
    PathTree* t = rafleTreeCalc(g, color, ind);
    if (t == emptyTree) {
        return IND_PB;
    }
    printf("\npresence of possible paths : %s\n", (pathTreeDepth(t) > 0) ? "true" : "false");
    if (pathTreeDepth(t) == 0) {
        return ind;
    }
    eatingTest(g, color, t);
    pathTreeFree(t);
    return IND_CHANGE_ALLOWED;
}

void testRafleTree(Game* g){
    bool is_white = g -> is_white;
    g -> ind_move = testRafleTreePmetre(g, is_white, g -> ind_move);
}