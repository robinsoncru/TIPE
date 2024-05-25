#ifndef COORD
#define COORD

#include "../debug/debug.h"

int dir(int a);
Uint8 getCodeFromDirs(int horizontalDir, int verticalDir);
void getDirsFromCode(Uint8 c, int* di, int* dj);
Coord sub(Coord ptA, Coord ptB);
Coord reduceNormOfOne(Coord v);
Coord add(Coord u, Coord v);
Coord mult(int k, Coord u);
void coordPrint(Coord x);
#endif