#include <stdlib.h>
#include <stdbool.h>

#ifndef PATH_TREE
#define PATH_TREE

#include "../../fundamental_functions/coord/coord.h"
#include "../../fundamental_functions/debug/debug.h"
/*Arbre de chemins pour les rafles.
C'est une structure reccursive qui consiste en un arbre de
coordonnees symbolisant un ensemble de chemins sur le plateau.
L'arbre est d'arite maximale 3 (4 en realite)*/



PathTree* pathTreeCreateNode(int i, int j);
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

void pathTreeFree(PathTree* t);
    //Libere l'integralite de l'abre de chemin
    //s'il est non-vide

PathTree* pathTreeChild(PathTree* t, int hDir, int vDir);
    //Renvoie le noeud consistant a aller dans la direction indiquee par les deux
    //entiers en argument. Cette fonction n'a pas d'effet de bords.
    //Renvoie NULL si on ne peut pas aller dans ladite direction

void pathTreeConnect(PathTree* parent, PathTree* child, int hDir, int vDir);
    //connecte le parent fournit a l'enfant donne selon la direction en argument
    //cette fonction a des effets de bords.

Coord pathTreeLabel(PathTree* t);

void pathTreeGetCoord(PathTree* t, int* i, int* j);

int pathTreeDepth(PathTree* t);
int pathTreeNBChilds(PathTree* t);
uint8_t pathTreeFirstChild(PathTree* t);

void pathTreeEmptyChild(PathTree* t, int hDir, int vDir);
#endif