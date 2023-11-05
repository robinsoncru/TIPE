#ifndef PATH_TREE_DISPLAY
#define PATH_TREE_DISPLAY
#include "../../fundamental_functions/interface/interface.h"
#include "../calc/path_tree.h"

void pathTreeDisplay(SDL_Renderer* render, Case damier[NB_CASE_LG][NB_CASE_LG], PathTree* t);
#endif