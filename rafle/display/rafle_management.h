#include "path_tree_display.h"
#include "../calc/rafle_tree_calc.h"
#include "../calc/path_tree.h"
#include <SDL2/SDL_render.h>

#ifndef RAFLE_MANAGEMENT
#define RAFLE_MANAGEMENT
void displayCurrentRafle(Game* g, SDL_Renderer* render, PathTree* cacheTree, bool display_tree);
void loadCurrentRafle(Game* g, PathTree** cacheTree);
void freeCurrentRafle(PathTree** cacheTree);
void manageCurrentRafleOnZUp(Game* g, PathTree** cacheTree, bool* display_tree);
#endif