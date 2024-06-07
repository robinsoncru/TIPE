#include "on_input.h"
#include "input_command.h"
#include <SDL2/SDL_keycode.h>

// called when the player makes an input
void onInput(Game *g, GraphicCache *cache, SDL_Event event)
{
    // if (event.type == SDL_MOUSEBUTTONDOWN
    //     || (event.type == SDL_KEYUP && event.key.keysym.sym != SDLK_z)) {
    //     cache->display_tree = false;
    //     if (cache->cacheTree != emptyTree) {
    //         freeCurrentRafle(&(cache->cacheTree));
    //     }
    // }

    switch (event.type)
    {

    // Select with the mouse
    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            onLMBDown(g, cache);
        }
        else if (event.button.button == SDL_BUTTON_RIGHT)
        {
            /* Pour les clics droit (prefere une double verif qu'un simple else)*/
            onRMBDown(g, cache);
            // onRUp(g, cache);
        }
        else
        {
            printf("ni gauche ni droite");
            assert(false);
        }
        break;

    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            // on avance le pion a gauche
            onLeftUp(g, cache, false);
            break;

        case SDLK_RIGHT:
            // on avance le pion a droite
            onRightUp(g, cache, false);
            break;

        case SDLK_UP:
            // test des rafles
            onUpUp(g, cache);
            break;

        case SDLK_ESCAPE:
            // quits the game
            onEscapeUp(g, cache);
            break;

        case SDLK_p:

            /* Promote the selected pawn */
            onPUP(g, cache, false);
            break;

        case SDLK_j:
            // Debuggeur: affiche l'etat des pions
            onJUP(g, cache);
            break;
            // les autres fonctions restent a implementer

        case SDLK_h:
            // Debuggeur: affiche l'etat du damier
            onHUP(g, cache);
            break;

        case SDLK_b:
            // Bidepl le pion
            onBUP(g, cache, false);
            break;

        case SDLK_l:
            onLUP(g);
            break;

        case SDLK_a:
            onAUP(cache);
            break;

        case SDLK_z:
            onZUp(g, cache);
            // affiche l'arbre de rafle
            break;
        case SDLK_k:
            onKUp(g, cache);
            break;

        case SDLK_r:
            onRUp(g, cache);

            break;

        case SDLK_w:
            onWUp(g, cache);
            break;

        case SDLK_q:
            onQUp(g);
            break;

        default:
            printv("Aucune touche reconnue");
            assert(false);
        }
        break;

    default:
        break;
    }
}