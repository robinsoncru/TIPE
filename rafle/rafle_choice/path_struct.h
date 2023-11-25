#ifndef PATH_STRUCT
#define PATH_STRUCT

#include <stdlib.h>
#include "four_pack.h"

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

Path* pathCreate(unsigned int c);
void pathFree(Path* path);
void pathAdd(Coord x, Path* path);
Coord pathPeekFirstIn(Path* path);
Coord pathPeekLastIn(Path* path);
Coord pathPopFirstIn(Path* path);
Coord pathPopLastIn(Path* path);

void pathPrint(Path* path);
void pathTabPrint(Path* path);
#endif //PATH_STRUCT