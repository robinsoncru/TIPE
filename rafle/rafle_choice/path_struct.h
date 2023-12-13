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
void pathAdd(Uint8 x, Path* path);
Uint8 pathPeekFirstIn(Path* path);
Uint8 pathPeekLastIn(Path* path);
Uint8 pathPopFirstIn(Path* path);
Uint8 pathPopLastIn(Path* path);

void pathPrint(Path* path);
#endif //PATH_STRUCT