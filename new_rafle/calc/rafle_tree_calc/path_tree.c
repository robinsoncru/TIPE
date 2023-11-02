#include "path_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct PathTree{
    int depth;
    Coord point;
    struct PathTree* childs[ARITE];
} PathTree;

PathTree* emptyTree = NULL;

PathTree* pathTreeCreateNode(int i, int j){
    //Cree un noeud de chemin correspondant
    //a la case de coordonnees (i, j)
    //Il est tout a fait possible d'avoir 2 noeuds
    //differents avec la meme etiquette, c'est meme volontaire
    //pour faciliter le parcour des chemins dans le cas de
    //trajets en losange comme ci-dessous :
    //   o
    //__/ \__
    //  \ /
    //   o
    PathTree* res = malloc(sizeof(PathTree));
    res -> depth = 0;
    Coord point;
    point.i = i;
    point.j = j;
    res -> point = point;
    for (int i = 0; i < ARITE; i++) {
        res -> childs[i] = emptyTree;
    }
    return res;
}

void pathTreeFree(PathTree* pathTree){
    //Libere l'integralite de l'abre de chemin
    if (pathTree != emptyTree) {
        for (int i = 0; i < ARITE; i++) {
        pathTreeFree(pathTree -> childs[i]);
        }
    }
    free(pathTree);
}

PathTree* pathTreeChild(PathTree* pathTree, int horizontalDir, int verticalDir){
    //Renvoie le noeud consistant a aller dans la direction indiquee par les deux
    //entiers en argument. Cette fonction n'a pas d'effet de bords.
    //Renvoie NULL si on ne peut pas aller dans ladite direction
    int index = getCodeFromDirs(horizontalDir, verticalDir);
    return pathTree -> childs[index];
}

void pathTreeConnect(PathTree* parent, PathTree* child, int horizontalDir, int verticalDir){
    
    //connecte le parent fournit a l'enfant donne selon la direction en argument
    //cette fonction a des effets de bords.
    int index = getCodeFromDirs(horizontalDir, verticalDir);
    parent -> childs[index] = child;

    int depthParent = parent -> depth;
    int depthChild = child -> depth;
    if (depthChild + 1 > depthParent) {
        parent -> depth = depthChild + 1;
    }
}

Coord pathTreeLabel(PathTree* node){
    assert(node != emptyTree);
    return node -> point;
}

void pathTreeGetCoord(PathTree* t, int* i, int* j){
    Coord c = pathTreeLabel(t);
    *i = c.i;
    *j = c.j;
}

int pathTreeDepth(PathTree* pathTree){
    if (pathTree == emptyTree) {
        return -1;
    }
    return pathTree -> depth;
}