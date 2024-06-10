#ifndef PLAY_A_MOVE
#define PLAY_A_MOVE

#include "debug_autoplay.h"
#include "../../AI/move_simulation/applyMoveAI.h"
#include "../../AI/move_for_sure/applyMoveForSure.h"

int play_a_move(int move, int ind_pawn, Game *g, GraphicCache *cache, int nb_coups, int *l_coups, int *l_depl, bool isNGE);
int automaticRandomPlay(Game* g, GraphicCache* cache, int nbCoups);
int random_index(Game *g);
int plannifier_index(Game *g, int nb_coups, int* l_coups);
int plannifier_index_color(int nb_coups, int *l_coups, bool color);
int random_play();
Move extract_random_move_from_tab_move(MoveTab *t);
int extract_pmetre_from_move(Move m);

#endif