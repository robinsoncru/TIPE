#include "../Calculus/calculus_functions.h"
#include "../precondition/precondition.h"



/* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value);
int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get);
pawn get_pawn(Game *g, bool color, int ind);

void put_case_damier(Game *g, int i, int j, int whichPmetre, int valeur);
Case get_case_damier(Game *g, int i, int j);