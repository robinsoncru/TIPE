#ifndef PLAY_A_MOVE
#define PLAY_A_MOVE

#include "../actions/input_command.h"

void play_a_move(int move, int ind_pawn, Game *g, GraphicCache *cache);
int random_index(Game *g);
int plannifier_index(Game *g, int nb_coups, int* l_coups);
int random_play();

#endif