#ifndef RAFLE_EATING
#define RAFLE_EATING

#include "../../fundamental_functions/game_functions/game_functions.h"
#include "../calc/path_tree.h"

void pawnEat(Game* g, int indEater, bool is_white, int di, int dj);
void eatRafle(Game* g, int indEater, bool is_white, PathTree* t, Path* r);
Path* lazyRafle(PathTree* t);
#endif //RAFLE_EATING