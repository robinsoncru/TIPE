#include "coord.h"

Coord sub(Coord ptA, Coord ptB){
    Coord res;
    res.i = ptA.i - ptB.i;
    res.j = ptA.j - ptB.j;

    return res;
}