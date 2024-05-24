#ifndef LISTING_CHECKS
#define LISTING_CHECKS

#include "../../../fundamental_functions/game_functions/access_functions/access_functions.h"

bool isFriendable(Game* g, int selectedPawn, bool is_white);
bool isEnnemiable(Game* g, int selectedPawn, bool is_white);
bool isPromotable(Game* g, int selectedPawn, bool is_white);
#endif