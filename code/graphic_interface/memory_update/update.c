#include "update.h"

// called every frame.
// updates the memory, useful to manage stuff that must be done
// or displayed for a certain time
int update(Game *g, GraphicCache *cache, int nb_coups, int *l_coups, int *l_depl, bool isNGE)
{

    // increments all chronometers
    for (int i = 0; i < TICKS_ARRAY_LENGTH; i++)
    {
        if (cache->ticksArray[i] > 0)
        {
            cache->ticksArray[i]++;
        }
    }

    // creates a transition between the turns
    if (cache->ticksArray[CHANGE_TICKS] > MAX_TICKS)
    {
        cache->ticksArray[CHANGE_TICKS] = 0;

        // change_damier(g);
        cache->txtMessage->display = false;
        g->indCheck = IND_NORMAL;
    }

    // tells if the player has not selected a pawn and tries
    // to move no matter what
    if (cache->ticksArray[ERROR_TICKS] > MAX_TICKS)
    {
        cache->ticksArray[ERROR_TICKS] = 0;

        cache->txtMessage->display = false;
        g->indCheck = IND_NORMAL;
    }

    // Control pannel
    if (SDL_PollEvent(&(cache->event)) == 1)
    {
        switch (cache->event.type)
        {

        case SDL_QUIT:
            // Exit the game
            cache->is_playing = false;
            break;

        default:
            // called if any input is made
            onInput(g, cache, cache->event);
            break;
        }
    }
    else
    {
        // Autoplay take over
        if (cache->autoplay)
        {
            if (g->is_white)
            {
                nb_coups = play_a_move(VOID_INDEX, VOID_INDEX, g, cache, nb_coups, l_coups, l_depl, true);
            }
            else
            {
                nb_coups = play_a_move(VOID_INDEX, VOID_INDEX, g, cache, nb_coups, l_coups, l_depl, isNGE);
            }
            // print_pawns(g, g->is_white);
        }
    }
    return nb_coups;
}