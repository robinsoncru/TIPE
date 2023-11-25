#ifndef LOGIC_FUNCTIONS
#define LOGIC_FUNCTIONS

#include "calculus_functions.h"

// Logic functions
bool freeCase(Case c);
bool basicChecks(Game *g);
/* Couronne un pion en dame et s'il avait un ennemi, celui ci meurt */
bool inGame(int lig, int col);
bool outOfBounds(int i, int j);
bool eatingIsOutOfBounds(int i, int j, int add0, int add1);
bool canMove(Game *g, bool is_white, int ind, bool left);
bool canBePromoted(Game *g, bool is_white, int ind);
bool diagIsFree(Game *g, Coord initPos, Coord finalPos);
bool queenCanMove(Game *g, bool is_white, int ind, Coord finalPos);
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
bool moveBackAvailable(Game *g);
bool canMoveBack(Game *g, bool is_white, int ind, bool left);
bool needPutMoveBack(Game *g);

bool canPromotion(Game *g);

bool canBeFriend(Game *g, int ind, bool color, Case c);
bool canBeEnnemy(Game *g, int ind, bool color, Case c);

bool canBiDepl(Game *g, int ind, bool color);
bool isInCloud(Game *g, bool color, int ind);
bool canStormBreaks(Game *g, int ind, int color);
bool canStormBreaksForTheOthers(Game *g, int ind, int color);

#endif // LOGIC_FUNCTIONS