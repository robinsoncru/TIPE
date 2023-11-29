#ifndef GAME_MANAGEMENT
#define GAME_MANAGEMENT

#include "../aux_game_functions/aux_functions.h"

void moveBackGameManagement(Game *g);
void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack);
void startTurnGameManagement(Game *g);


#endif // GAME_MANAGEMENT
