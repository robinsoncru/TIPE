#include "coord.h"

Coord sub(Coord ptA, Coord ptB)
{
    Coord res;
    res.i = ptA.i - ptB.i;
    res.j = ptA.j - ptB.j;

    return res;
}

Coord add(Coord u, Coord v)
{
    Coord res = {
        u.i + v.i,
        u.j + v.j};
    return res;
}

Coord mult(int k, Coord u)
{
    Coord res = {k * u.i, k * u.j};
    return res;
}

// la direction est donnee sous la forme d'un couple
//(dx, dy) ou dx et dy peuvent valoir -1 ou 1.
// On lui associe un nombre entre 0 et 3 inclus
// dont le bit de poids faible est la direction horizontale
// et le bit de poids fort la direction verticale.
// 0 sens negatif, 1 sens positif
Uint8 getCodeFromDirs(int dj, int di)
{

    Uint8 weak = (dj == 1) ? 1 : 0;
    Uint8 strong = (di == 1) ? 1 : 0;
    return (strong << 1) | (weak);
}
int dir(int a)
{
    return (a == 0) ? -1 : 1;
}

void getDirsFromCode(Uint8 c, int *di, int *dj)
{
    *dj = dir(c % 2);
    *di = dir((c >> 1) % 2);
}

void coordPrint(Coord x)
{
    printf("(%d, %d)", x.i, x.j);
}