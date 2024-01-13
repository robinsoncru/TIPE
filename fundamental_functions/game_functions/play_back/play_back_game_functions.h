#ifndef PLAY_BACK_GAME_FUNCTIONS
#define PLAY_BACK_GAME_FUNCTIONS


#include "../management/game_management.h"


// cancel a move, turn the game for the other player and cancel the changes for the move (queen promotion ...): liste chaine qui contient les transfo durant les moves

void pawnMovedBack(Game *g, bool is_white, int ind, bool left);

Game *record_game(Game *g);

void annuler_coup(Game **g, Game *previous_g);

// void moveBack(Game *g, Kmaillon *l); // ?

// void promotionBack(Game *g, Kmaillon *l);

// void lienAmitieBack(int lig, int col, Game *g, Kmaillon *l);

// void lienEnnemitieBack(int lig, int col, Game *g, Kmaillon *l);

// void biDeplBack(Game *g, int ind, bool color, Kmaillon *l);

// void queenDeplBack(Game *g, int ind, bool color, queen_move_t tuple_coord, Kmaillon *l);

#endif