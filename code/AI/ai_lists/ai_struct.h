#ifndef AI_STRUCT
#define AI_STRUCT

#include "../../fundamental_functions/game_functions/Source/source_header.h"

typedef struct AI{
    //quantifie le niveau d'avantage des blancs sur les noirs
    double (*analyse)(Game*);
    //fonction d'ecrasement
    double (*ecrasement)(double);
    //profondeur de recherche
    int depth;
}AI;
#endif