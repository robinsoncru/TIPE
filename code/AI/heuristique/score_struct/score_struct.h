#ifndef SCORE_STRUCT
#define SCORE_STRUCT

#include "../../../fundamental_functions/interface/interface.h"
#include <limits.h>

// typedef struct gameboard_t;

// struct gameboard_t {
    
// }

#define ABSOLUTE_VICTORY 10
#define ABSOLUTE_DEFEATE 0
#define VICTORY_MAJORITY 9
#define DEFEATE_MAJORITY 1
#define NO_SCORE_GIVEN -1

int value_to_queen(int value) {
    return value*value;
}

int value_to_friend(int value) {
    return 2*value;
}

int value_to_foe(int value) {
    return value_to_friend(value);
}

int value_to_ghost(int value) {
    return value/2;
}

/* Pour donner une valeur constante aux pièces, on peut définir un système de mesure dont l'unité est le 
 pion de départ */

#define CASUAL_PAWN_VALUE 4

#define CASUAL_QUEEN_VALUE value_to_queen(CASUAL_PAWN_VALUE)





#endif
