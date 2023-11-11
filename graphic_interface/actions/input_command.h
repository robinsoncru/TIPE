#ifndef INPUT_COMMAND
#define INPUT_COMMAND

#include "../memory_update/graphic_cache.h"
#include "../../quantum_rules/quantum_functions.h"

void onLMBDown(Game* g, GraphicCache* cache);
void onRMBDown(Game* g, GraphicCache* cache);
void onLeftUp(Game* g, GraphicCache* cache);
void onRightUp(Game* g, GraphicCache* cache);
void onEscapeUp(Game* g, GraphicCache* cache);

//void onAUp(Game* g, GraphicCache* cache);
//void onZUp(Game* g, GraphicCache* cache);
//void onRUp(Game* g, GraphicCache* cache);
#endif //INPUT_COMMAND