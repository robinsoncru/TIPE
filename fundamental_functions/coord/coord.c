#include "coord.h"

Coord sub(Coord ptA, Coord ptB){
    Coord res;
    res.i = ptA.i - ptB.i;
    res.j = ptA.j - ptB.j;

    return res;
}

int xMinusSing(int x){
    int sg = (x >= 0) ? 1 : -1;
    return x - sg;
}

Coord reduceNormOfOne(Coord v){
    Coord res;
    res.i = xMinusSing(v.i);
    res.j = xMinusSing(v.j);
    return res;
}