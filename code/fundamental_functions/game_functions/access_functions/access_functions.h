#ifndef ACCESS_FUNCTION
#define ACCESS_FUNCTION

#include "../Calculus/calculus_functions.h"
#include "../precondition/precondition.h"



/* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */
void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value);
int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_get);
pawn get_pawn(Game *g, bool color, int ind);

void put_case_damier(Game *g, int i, int j, int whichPmetre, int valeur);
Case get_case_damier(Game *g, int i, int j);


void putFriendByInd(Game *g, int indActu, int indFriend, bool colorActu, bool valeur);
bool getFriendByInd(Game *g, int indActu, int indFriend, bool colorActu);

coord_tab_t *friendTabToCoordTab(Game *g, int ind, bool color);

bool has_friend(Game *g, int ind, int color);
float zero_fun(Game *g);
Coord to_coord(int i, int j);
#endif