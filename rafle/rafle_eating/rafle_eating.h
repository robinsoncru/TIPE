#ifndef RAFLE_EATING
#define RAFLE_EATING

#include "../../fundamental_functions/game_functions/game_functions.h"
#include "../calc/path_tree.h"

void eatRafle(Game* g, int indEater, bool is_white, PathTree* t, Path* r);
Path* lazyRafle(PathTree* t);
#endif //RAFLE_EATING