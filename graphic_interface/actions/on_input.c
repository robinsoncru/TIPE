#include "on_input.h"
#include "input_command.h"
#include <SDL2/SDL_keycode.h>

//called when the player makes an input
void onInput(Game* g, GraphicCache* cache, SDL_Event event){
    //if (event.type == SDL_MOUSEBUTTONDOWN
    //    || (event.type == SDL_KEYUP && event.key.keysym.sym != SDLK_z)) {
    //    cache->display_tree = false;
    //    if (cache->cacheTree != emptyTree) {
    //        freeCurrentRafle(&(cache->cacheTree));
    //    }
    //}

    switch (event.type) {
    
        //Select with the mouse
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                // onLMBDown(g, cache);
            }
        break;
        
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    //on avance le pion a gauche
                    onLeftUp(g, cache);
                break;

                case SDLK_RIGHT:
                    //on avance le pion a droite
                    onRightUp(g, cache);
                break;

                case SDLK_ESCAPE:
                    //quits the game
                    onEscapeUp(g, cache);
                break;
                //les autres fonctions restent a implementer
            }

    }
}