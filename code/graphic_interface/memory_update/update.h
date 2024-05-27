#ifndef UPDATE
#define UPDATE

#include "graphic_cache.h"
#include "../actions/on_input.h"
#include "../machine_plays_functions/basic_play_function.h"

//called every tick.
//updates the memory
int update(Game *g, GraphicCache *cache, int nb_coups, int *l_coups, int *l_depl);
#endif //UPDATE