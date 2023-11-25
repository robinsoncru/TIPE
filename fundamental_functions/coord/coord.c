#include "coord.h"

Coord sub(Coord ptA, Coord ptB){
    Coord res;
    res.i = ptA.i - ptB.i;
    res.j = ptA.j - ptB.j;

    return res;
}

Coord add(Coord u, Coord v){
    Coord res = {
        u.i + v.i,
        u.j + v.j
    };
    return res;
}

Coord mult(int k, Coord u){
    Coord res = {k*u.i, k*u.j};
    return res;
}

void coordPrint(Coord x){
    printf("(%d, %d)", x.i, x.j);
}