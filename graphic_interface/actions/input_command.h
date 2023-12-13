#ifndef INPUT_COMMAND
#define INPUT_COMMAND

#include "../../fundamental_functions/interface/interface.h"
#include "../memory_update/graphic_cache.h"
<<<<<<< HEAD
=======
#include "../../rafle/rafle.h"
>>>>>>> 743c96089ebfa5faab12ccbb8db456e91aefee77
#include <SDL2/SDL_events.h>
#include <stdio.h>

void onLMBDown(Game* g, GraphicCache* cache);
void onRMBDown(Game* g, GraphicCache* cache);
void onLeftUp(Game* g, GraphicCache* cache);
void onRightUp(Game* g, GraphicCache* cache);
void onEscapeUp(Game* g, GraphicCache* cache);
void onUpUp(Game* g, GraphicCache* cache);
void onPUP(Game *g, GraphicCache *cache);
void onJUP(Game *g, GraphicCache *cache);
void onHUP(Game *g, GraphicCache *cache);
void onBUP(Game *g, GraphicCache *cache);
void onLUP(Game *g);
<<<<<<< HEAD

=======
void onZUp(Game* g, GraphicCache* cache);
void onKUp(Game* g, GraphicCache* cache);
>>>>>>> 743c96089ebfa5faab12ccbb8db456e91aefee77
//void onAUp(Game* g, GraphicCache* cache);
//void onRUp(Game* g, GraphicCache* cache);
#endif //INPUT_COMMAND