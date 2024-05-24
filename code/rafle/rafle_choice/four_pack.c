#include "four_pack.h"
#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include "../../fundamental_functions/debug/debug.h"
//des hypotheses de bon usage son systematiquement faites
//d'un point de vue abstrait, represente des tableaux de
//4 directions au plus
//les directions sont numerotee
//du bit de pois fort au bit de poids faible
//en partant de 0

Uint8 fourPackGet(int k, FourPack fp){
    assertAndLog(k < 4 && k >= 0, "invalid index in fourPack");
    Uint8 shift = 6 - (k << 1);
    return (fp >> shift) % 4;
}

void fourPackSet(Uint8 c, int k, FourPack *fp){
    assertAndLog(k < 4 && k >= 0, "invalid index in fourPack");
    Uint8 del = fourPackGet(k, *fp);
    Uint8 shift = 6 - (k << 1);
    *fp = *fp - (del << shift) + (c << shift);
}