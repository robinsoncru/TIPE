#ifndef backwardMoveTab_STRUCT
#define backwardMoveTab_STRUCT

#include <stdbool.h>

typedef struct backwardMove_t {
    int indMovedPawn;
    int dir;
} backwardMove_t;

typedef struct backwardMoveTab_t {
    int n; //tab size
    backwardMove_t* tab;
} backwardMoveTab_t;

#endif