#ifndef ON_INPUT
#define ON_INPUT

#include "../../fundamental_functions/game_functions/game_functions.h"
#include "../../fundamental_functions/interface/interface.h"
#include "../memory_update/graphic_cache.h"
#include "input_command.h"
#include "../autoplay_functions/basic_play_function.h"

void onInput(Game* g, GraphicCache* cache, SDL_Event event);
#endif //ON_INPUT