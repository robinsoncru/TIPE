#ifndef LOGIC_FUNCTIONS
#define LOGIC_FUNCTIONS

#include "../fonctions_correctes/fonctions_correctes.h"

// Logic functions
bool canMove(Game *g, bool is_white, int ind, bool left);
bool canBePromoted(Game *g, bool is_white, int ind);
bool diagIsFree(Game *g, Coord initPos, Coord finalPos);
bool queenCanMove(Game *g, bool is_white, int ind, Coord finalPos);
bool canEat(Game *g, bool color, int ind, int i, int j, int add0, int add1);
bool moveBackAvailable(Game *g);
bool canMoveBack(Game *g, bool is_white, int ind, bool left);
bool needPutMoveBack(Game *g);

bool caseIsAccessible(Game* g, bool is_white, int i, int j);

bool canPromotion(Game *g);

bool canBeFriend(Game *g, int ind, bool color, Case c);
bool canBeEnnemy(Game *g, int ind, bool color, Case c);

bool canBiDepl(Game *g, int ind, bool color);
bool canStormBreaks(Game *g, int ind, int color);
bool canStormBreaksForTheOthers(Game *g, int ind, int color);


#endif // LOGIC_FUNCTIONS