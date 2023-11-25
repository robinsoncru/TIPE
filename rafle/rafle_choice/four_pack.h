#ifndef FOUR_PACK
#define FOUR_PACK

#include "../../fundamental_functions/fundamental_functions.h"
#include <stdlib.h>

//pack of four directions
//les directions sont numerotee
//du bit de pois fort au bit de poids faible
//en partant de 0
typedef Uint8 FourPack;

Uint8 fourPackGet(int k, FourPack fp);
void fourPackSet(Uint8 c, int k, FourPack* fp);

#endif //FOUR_PACK