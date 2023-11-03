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

void validIndexTest(int hDir, int vDir){
    bool hDirIsValid = hDir == -1 || hDir == 1;
    bool vDirIsValid = vDir == -1 || vDir == 1;
    assertAndLog(hDirIsValid && vDirIsValid, "Direction inexistante");
}

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
    assertAndLog(!outOfBounds(i, j), "Position inexistante");
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

void pathTreeFree(PathTree* t){
    //Libere l'integralite de l'abre de chemin
    if (t != emptyTree) {
        for (int i = 0; i < ARITE; i++) {
        pathTreeFree(t -> childs[i]);
        }
    }
    free(t);
}

PathTree* pathTreeChild(PathTree* t, int hDir, int vDir){
    //Renvoie le noeud consistant a aller dans la direction indiquee par les deux
    //entiers en argument. Cette fonction n'a pas d'effet de bords.
    //Renvoie NULL si on ne peut pas aller dans ladite direction
    assertAndLog(t != emptyTree, "Recherche d'enfant de l'arbre vide");
    validIndexTest(hDir, vDir);

    int index = getCodeFromDirs(hDir, vDir);
    return t -> childs[index];
}

int pathTreeDepth(PathTree *t);
void pathTreeConnect(PathTree* parent, PathTree* child, int hDir, int vDir){
    //connecte le parent fournit a l'enfant donne selon la direction en argument
    //cette fonction a des effets de bords.
    validIndexTest(hDir, vDir);

    int index = getCodeFromDirs(hDir, vDir);
    parent -> childs[index] = child;

    int depthParent = parent -> depth;
    int depthChild = pathTreeDepth(child);
    if (depthChild + 1 > depthParent) {
        parent -> depth = depthChild + 1;
    }
}

Coord pathTreeLabel(PathTree* t){
    assertAndLog(t != emptyTree, "Recherche d'etiquette de l'arbre vide");
    return t -> point;
}

void pathTreeGetCoord(PathTree* t, int* i, int* j){
    Coord c = pathTreeLabel(t);
    *i = c.i;
    *j = c.j;
}

int pathTreeDepth(PathTree* t){
    if (t == emptyTree) {
        return -1;
    }
    return t -> depth;
}

void pathTreeEmptyChild(PathTree* t, int hDir, int vDir){
    printf("\npathTreeEmptyChild called\n");
    //remplace l'enfant indique par l'arbre vide en le liberant au passage
    PathTree* m = pathTreeChild(t, hDir, vDir);
    pathTreeConnect(t, emptyTree, hDir, vDir);
    pathTreeFree(m);
    printf("\npathTreeEmptyChild works\n");
}