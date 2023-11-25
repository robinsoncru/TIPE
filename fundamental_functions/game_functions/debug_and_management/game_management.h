#ifndef GAME_MANAGEMENT
#define GAME_MANAGEMENT

#include "debug_functions.h"

void moveBackGameManagement(Game *g);
void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack);
void startTurnGameManagement(Game *g, bool doMove);


#endif // GAME_MANAGEMENT
