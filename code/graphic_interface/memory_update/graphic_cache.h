#ifndef CACHE_affichage
#define CACHE_affichage

#include "../../fundamental_functions/affichage/affichage.h"
#include "../rafle_display/path_tree_display.h"




//graphic cache functions

//initializes the cache.
//In case of an error, interrupts the process
//and returns a cache with EXIT_FAILURE as a status
GraphicCache* initCache();

void freeCache(GraphicCache* cache);
//called every tick to display stuff on the screen
//depending on the state of the cache
//DOES NOT modify the cache
void display(Game* g, GraphicCache* cache);

//displays an alert on the screen
//and does other stuff depending of the chosen timer
void alert(GraphicCache* cache, int alertIndex, int timerIndex);
#endif //CACHE_affichage