#ifndef INPUT_COMMAND
#define INPUT_COMMAND

#include "../interface/interface.h"
#include "../memory_update/graphic_cache.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>

void onLMBDown(Game* g, GraphicCache* cache);
void onRMBDown(Game* g, GraphicCache* cache);
void onLeftUp(Game* g, GraphicCache* cache);
void onRightUp(Game* g, GraphicCache* cache);
void onEscapeUp(Game* g, GraphicCache* cache);
void onPUP(Game *g, GraphicCache *cache);
void onJUP(Game *g, GraphicCache *cache);
void onHUP(Game *g, GraphicCache *cache);
void onBUP(Game *g, GraphicCache *cache);
void onLUP(Game *g);

//void onAUp(Game* g, GraphicCache* cache);
//void onZUp(Game* g, GraphicCache* cache);
//void onRUp(Game* g, GraphicCache* cache);
#endif //INPUT_COMMAND