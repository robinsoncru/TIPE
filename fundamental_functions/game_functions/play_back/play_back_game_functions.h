#ifndef PLAY_BACK_GAME_FUNCTIONS
#define PLAY_BACK_GAME_FUNCTIONS


#include "../management/game_management.h"


// cancel a move, turn the game for the other player and cancel the changes for the move (queen promotion ...): liste chaine qui contient les transfo durant les moves


void pawnMoveNGE(Game *g, bool is_white, int ind, bool left);

void pawnMoveCancel(Game *g, bool is_white, int ind, bool left);

void pawnMoveBackNGE(Game *g, bool is_white, int ind, bool left);

void recreateCloud(Game *g, cloud_chain *l, ind_pba_t *survivor, bool iw);
ind_bool_t biDeplNGE(Game *g, bool color, int ind);
void cancelBidepl(Game *g, int oldInd, ind_bool_t data);
int promotionNGE(Game *g, int ind);
void cancelPromotion(Game *g, int ind_old_friend, int ind_new_foe);

primary_data_t queenDeplNGE(Game *g, int ind, bool color, queen_move_t tuple_coord);
void cancelDeplQueen(Game *g, int ind_queen, queen_move_t tuple_coord, primary_data_t data, Coord init_coord);

#endif