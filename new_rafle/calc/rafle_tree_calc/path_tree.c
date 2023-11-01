#include "path_tree.h"
#include <stdlib.h>

int convert(int horizontalDir, int verticalDir){
    //la direction est donnee sous la forme d'un couple
    //(dx, dy) ou dx et dy peuvent valoir -1 ou 1
    //on lui associe un nombre entre 0 et 3 inclus
    //dont le bit de poids faible est la direction horizontale
    //et le bit de poids fort la direction verticale
    //0 sens negatif, 1 sens positif
    int res = 1;
    int weak = (horizontalDir == 1) ? 1 : 0;
    int strong = (verticalDir == 1) ? 1 : 0;
    return (strong << 1)|(weak);
}

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
    int index = convert(horizontalDir, verticalDir);
    return pathTree -> childs[index];
}

void pathTreeConnect(PathTree* parent, PathTree* child, int horizontalDir, int verticalDir){
    //connecte le parent fournit a l'enfant donne selon la direction en argument
    //cette fonction a des effets de bords.
    int index = convert(horizontalDir, verticalDir);
    parent -> childs[index] = child;

    int depthParent = parent -> depth;
    int depthChild = child -> depth;
    if (depthChild + 1 > depthParent) {
        parent -> depth = depthChild + 1;
    }
}

Coord pathTreeLabel(PathTree* node){
    return node -> point;
}

int pathTreeDepth(PathTree* pathTree){
    return pathTree -> depth;
}