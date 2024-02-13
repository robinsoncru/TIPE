#ifndef GAME_MANAGEMENT
#define GAME_MANAGEMENT

#include "../aux_game_functions/aux_functions.h"

void moveBackGameManagement(Game *g);
void startTurnGameManagement(Game *g);
void endTurnGameManagementSimple(Game *g, int pawn_played);
void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack);
#endif // GAME_MANAGEMENT
