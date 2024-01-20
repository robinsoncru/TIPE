#ifndef PLAY_BACK_GAME_FUNCTIONS
#define PLAY_BACK_GAME_FUNCTIONS


#include "../management/game_management.h"


// cancel a move, turn the game for the other player and cancel the changes for the move (queen promotion ...): liste chaine qui contient les transfo durant les moves


int pawnMoveNoGraphicEffect(Game *g, bool is_white, int ind, bool left);

void pawnMoveBackNoGraphicEffect(Game *g, bool is_white, int ind, bool left);

int biDeplNoGraphicEffect(Game *g, bool color, int ind);

#endif