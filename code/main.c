#include "main.h"
#include "fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include "test_config/test_config.h"

int quit(Game *g, GraphicCache *cache)
{
    freeCache(cache);
    free_game(g);
    TTF_Quit();
    SDL_Quit();
    return cache->statut;
}

void test_start_game(Game *g, GraphicCache *cache)
{
    // très pratique pour promouvoir une dame en qq coups

    



    // print_pawns(g, true);
    // print_pawns(g, false);
    // print_liensAmitie(g);
    print_state_game(g);
    // print_liensAmitie(g);
}

int main(int argc, char *argv[])
{
    // init the game
    Game *g = create_game();
    // init the graphic cache
    GraphicCache *cache = initCache();
    configGrandAtome(g);
    test_start_game(g, cache);
    // moveBackListingTest(g, cache);
    // alphaBetaPrunningTest(g);
    // end debug

    if (cache->statut != EXIT_SUCCESS)
    {
        return quit(g, cache);
    }

    cache->last_time = SDL_GetTicks();
    int nb_coups = 0;
    int l_coups[40] = {0, 3, 0, 3, 0, 0, 0};
    int l_depl[20] = {0, 14, 6, 6, 14, 0, 0, 8, 0, 14, 12, 0, 14, 0};

    // Start the game
    while (cache->is_playing)
    {
        cache->time_now = SDL_GetTicks();

        if (cache->time_now - cache->last_time > FRAME)
        {
            cache->last_time = cache->time_now;

            // clear the window
            SDL_RenderClear(cache->draw);

            // display the data in the cache and the game
            display(g, cache);

            // Rafraichit l'ecran
            SDL_RenderPresent(cache->draw);

            // update the data in the cache
            nb_coups = update(g, cache, nb_coups, l_coups, l_depl, true);
        }
    }
    return quit(g, cache);
}