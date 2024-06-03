#ifndef backwardMoveTab_INTERFACE
#define backwardMoveTab_INTERFACE

#include "backwardMoveTab_struct.h"
#include <stdbool.h>

backwardMoveTab_t* backwardMoveTabCreate(int c);
backwardMoveTab_t* backwardMoveTabCopy(backwardMoveTab_t* t);
void backwardMoveTabSetIndMovedPawn(backwardMoveTab_t* t, int i, int newInd);
void backwardMoveTabSetDir(backwardMoveTab_t* t, int i, int newDir);
int backwardMoveTabGetIndMovedPawn(backwardMoveTab_t* t, int i);
int backwardMoveTabGetDir(backwardMoveTab_t* t, int i);
void backwardMoveTabFree(backwardMoveTab_t* t);
#endif