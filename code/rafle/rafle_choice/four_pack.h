#ifndef FOUR_PACK
#define FOUR_PACK

#include "../../fundamental_functions/coord/coord.h"
#include <SDL2/SDL_stdinc.h>
#include <stdlib.h>

//pack of four directions
//les directions sont numerotee
//du bit de pois fort au bit de poids faible
//en partant de 0

Uint8 fourPackGet(int k, FourPack fp);
void fourPackSet(Uint8 c, int k, FourPack* fp);

#endif //FOUR_PACK