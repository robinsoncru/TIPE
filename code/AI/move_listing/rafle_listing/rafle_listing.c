#include "rafle_listing.h"

void listPathsOfTrees(PathTree* rafleTree, Path* currentPath, Move currentMove, Move* temporaryResult, int* nbMoves){
    int nbChilds = pathTreeNBChilds(rafleTree);

    Uint8 dirCode;
    int hDir, vDir;
    PathTree* currentChild;
    switch (nbChilds) {
        case 0:
            currentMove.rafle = pathCopy(currentPath);
            temporaryResult[*nbMoves] = currentMove;
            *nbMoves = *nbMoves + 1;
            break;
        
        case 1://il n'y a pas de choix a faire, donc on ajoute rien a la rafle
            dirCode = pathTreeFirstChild(rafleTree);
            getDirsFromCode(dirCode, &vDir, &hDir);
            currentChild = pathTreeChild(rafleTree, hDir, vDir);
            listPathsOfTrees(currentChild, currentPath, currentMove, temporaryResult, nbMoves);
            break;
        
        default: //2 enfants ou plus
            for (dirCode = 0; dirCode < 4; dirCode++) {
                getDirsFromCode(dirCode, &vDir, &hDir);
                currentChild = pathTreeChild(rafleTree, hDir, vDir);
                if (currentChild != emptyTree) {
                    Path* copy = pathCopy(currentPath);
                    pathAdd(dirCode, copy);
                    listPathsOfTrees(currentChild, copy, currentMove, temporaryResult, nbMoves);
                    pathFree(copy);
                }
            }
            break;
    }
}

void listRafles(Game* g, int selectedPawn, Coord tmpPos, Move* temporaryResult, int* nbMoves){
    //precalcul necessaire a l'enumeration des rafles
    pawn p = get_pawn(g, g->is_white, selectedPawn);
    int initI = p.lig, initJ = p.col;
    change_pawn_place(g, selectedPawn, g->is_white, tmpPos.i, tmpPos.j);
    PathTree* tree = rafleTreeCalc(g, g->is_white, selectedPawn);

    if (pathTreeDepth(tree) > 0) {
        Move currentMove;
        currentMove.type = rafleType;
        currentMove.manipulatedPawn = selectedPawn;
        currentMove.rafleTree = tree;

        Path* currentPath = pathCreate(pathTreeDepth(tree) + 1);
        listPathsOfTrees(tree, currentPath, currentMove, temporaryResult, nbMoves);
        pathFree(currentPath);
    }
    else {
        pathTreeFree(tree);
    }

    //retour a l'etat initial
    change_pawn_place(g, selectedPawn, g->is_white, initI, initJ);
}