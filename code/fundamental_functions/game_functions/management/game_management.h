#ifndef GAME_MANAGEMENT
#define GAME_MANAGEMENT

#include "../aux_game_functions/aux_functions.h"

bool endTurnGameManagementNGE(Game *g, int indMovedPawn, int indCheck, bool doMoveBack);
void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack);
void handlePawnGhostAndPromotion(Game *g, bool color, int ind);
#endif // GAME_MANAGEMENT
