#ifndef FPATH
#define FPATH


#include "fcoord_struct.h"
#define ARITE 4
#define NO_CHILD_ERROR 5



typedef struct PathTree PathTree;

typedef struct PathTree{
    int depth;
    Coord point;
    int nbChilds;
    struct PathTree* childs[ARITE];
} PathTree;


extern PathTree* emptyTree;

typedef Uint8 FourPack;



typedef struct Path {
    unsigned int n, c;
    //n : size
    //c : capacity
    int deb, fin;
    //     deb       fin
    //      v         v
    //[??? | contenu | ???]
    FourPack* tab;
} Path;

#endif