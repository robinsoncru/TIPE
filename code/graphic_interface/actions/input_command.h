#ifndef INPUT_COMMAND
#define INPUT_COMMAND

#include "../memory_update/graphic_cache.h"
#include "../../rafle/rafle.h"
#include "../../AI/move_listing/move_max.h"
#include "../../AI/move_listing/move_listing.h"
#include "../../AI/move_listing/move_listing_debug/print_moves.h"
#include "../../AI/alpha_beta_prunning/alpha_beta_prunning.h"
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
void onWUp(Game *g, GraphicCache *cache);


//void onAUp(Game* g, GraphicCache* cache);
//void onZUp(Game* g, GraphicCache* cache);

void onZUp(Game* g, GraphicCache* cache);
void onKUp(Game* g, GraphicCache* cache);
//void onAUp(Game* g, GraphicCache* cache);
void onRUp(Game* g, GraphicCache* cache);

void onQUp(Game* g);
//calcule et imprime le meilleur coup
//selon l'IA qui a la main

void checkLienAmitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch);
void checkLienEnnemitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch);
void checkQueenDepl(Game *g, GraphicCache *cache, bool iw, int lig, int col, bool autoplay);
void checkPawnMove(Game *g, GraphicCache *cache, bool left, bool autoplay);
void checkPawnMoveBack(Game *g, GraphicCache *cache, bool autoplay);
void checkBiDepl(Game *g, GraphicCache *cache, bool autoplay);
#endif //INPUT_COMMAND