#ifndef RAFLE_MANAGEMENT
#define RAFLE_MANAGEMENT
#include "path_tree_display.h"
void displayCurrentRafle(Game* g, SDL_Renderer* render, PathTree* cacheTree, bool display_tree);
void loadCurrentRafle(Game* g, PathTree** cacheTree);
void freeCurrentRafle(PathTree** cacheTree);
void manageCurrentRafleOnZUp(Game* g, PathTree** cacheTree, bool* display_tree);
#endif