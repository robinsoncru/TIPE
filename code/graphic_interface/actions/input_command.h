#ifndef INPUT_COMMAND
#define INPUT_COMMAND

#include "../memory_update/graphic_cache.h"
#include "../../rafle/rafle.h"
#include "../../AI/move_listing/move_max.h"
#include "../../AI/move_listing/move_listing.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>

void onLMBDown(Game* g, GraphicCache* cache);
void onRMBDown(Game* g, GraphicCache* cache);
void onLeftUp(Game* g, GraphicCache* cache, bool autoplay);
void onRightUp(Game* g, GraphicCache* cache, bool autoplay);
void onEscapeUp(Game* g, GraphicCache* cache);
void onUpUp(Game* g, GraphicCache* cache);
void onPUP(Game *g, GraphicCache *cache, bool autoplay);
void onJUP(Game *g, GraphicCache *cache);
void onHUP(Game *g, GraphicCache *cache);
void onBUP(Game *g, GraphicCache *cache, bool autoplay);
void onLUP(Game *g);
void onAUP(GraphicCache *cache);

//void onAUp(Game* g, GraphicCache* cache);
//void onZUp(Game* g, GraphicCache* cache);

void onZUp(Game* g, GraphicCache* cache);
void onKUp(Game* g, GraphicCache* cache);
//void onAUp(Game* g, GraphicCache* cache);
void onRUp(Game* g, GraphicCache* cache);

void checkLienAmitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch);
void checkLienEnnemitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch);
void checkQueenDepl(Game *g, GraphicCache *cache, bool iw, int lig, int col, bool autoplay);
#endif //INPUT_COMMAND