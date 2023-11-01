#include <stdlib.h>
#include <stdbool.h>
#define ARITE 4

/*Arbre de chemins pour les rafles.
C'est une structure reccursive qui consiste en un arbre de
coordonnees symbolisant un ensemble de chemins sur le plateau.
L'arbre est d'arite maximale 3 (4 en realite)*/

typedef struct Coord{
    int i;
    int j;
    /*
    i est l'abscisse
    j l'ordonnee
    Dessin du repere :
    |
    |
    |
    A
    |
    o-->-------
    l'origine (0, 0) est la case inferieure gauche du plateau.*/
} Coord;

typedef struct PathTree PathTree;

extern PathTree* emptyTree;

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

void pathTreeFree(PathTree* pathTree);
    //Libere l'integralite de l'abre de chemin
    //s'il est non-vide

PathTree* pathTreeChild(PathTree* pathTree, int horizontalDir, int verticalDir);
    //Renvoie le noeud consistant a aller dans la direction indiquee par les deux
    //entiers en argument. Cette fonction n'a pas d'effet de bords.
    //Renvoie NULL si on ne peut pas aller dans ladite direction

void pathTreeConnect(PathTree* parent, PathTree* child, int horizontalDir, int verticalDir);
    //connecte le parent fournit a l'enfant donne selon la direction en argument
    //cette fonction a des effets de bords.

Coord pathTreeLabel(PathTree* node);

int pathTreeDepth(PathTree* pathTree);