#ifndef CALCULUS_FUNCTIONS
#define CALCULUS_FUNCTIONS

#include "../debug/debug_functions.h"


// Calculus fonctions
int NON(int b);
int abs(int a);
Uint8 getCodeFromDirs(int dj, int di);
int dir(int a);
void getDirsFromCode(Uint8 c, int *di, int *dj);
bool int_to_bool(int integer);
int bool_to_int(bool b);

/* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value);
int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get);


#endif //CALCULUS FUNCTIONS