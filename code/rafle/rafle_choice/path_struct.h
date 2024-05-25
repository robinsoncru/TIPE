#ifndef PATH_STRUCT
#define PATH_STRUCT

#include <stdlib.h>
#include "four_pack.h"

Path* pathCreate(unsigned int c);
void pathFree(Path* path);
void pathAdd(Uint8 x, Path* path);
Path* pathCopy(Path* path);
Uint8 pathPeekFirstIn(Path* path);
Uint8 pathPeekLastIn(Path* path);
Uint8 pathPopFirstIn(Path* path);
Uint8 pathPopLastIn(Path* path);

void pathPrint(Path* path);
#endif //PATH_STRUCT