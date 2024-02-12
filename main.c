#include "main.h"

int quit(Game *g, GraphicCache *cache)
{
    freeCache(cache);
    free_game(g);
    TTF_Quit();
    SDL_Quit();
    return cache->statut;
}

void test_start_game(Game *g)
{
    // très pratique pour promouvoir une dame en qq coups
    for (int i = NB_PAWNS - 1; i > 2; i--)
    {

        killPawn(g, g->damier, g->allPawns[1][i].lig, g->allPawns[1][i].col);
        killPawn(g, g->damier, g->allPawns[0][i].lig, g->allPawns[0][i].col);
    }

    // Configuration triangle
    change_pawn_place(g->allPawns[1], g->damier, 0, 6, 6);
    change_pawn_place(g->allPawns[0], g->damier, 2, 5, 5);
    // change_pawn_place(g->allPawns[0], g->damier, 0, 5, 3);
    // change_pawn_place(g->allPawns[0], g->damier, 1, 3, 5);
    change_pawn_place(g->allPawns[0], g->damier, 1, 3, 3);
    change_pawn_place(g->allPawns[0], g->damier, 0, 3, 1);

    // Configuration move back + cloud
    // change_pawn_place(g->allPawns[1], g->damier, 0, 6, 6);
    // change_pawn_place(g->allPawns[1], g->damier, 1, 4, 4);
    // change_pawn_place(g->allPawns[0], g->damier, 1, 3, 5);
    // change_pawn_place(g->allPawns[0], g->damier, 2, 1, 1);
}

int main(int argc, char *argv[])
{
    // init the game
    Game *g = create_game();

    // int allMoves[4][2] = {{LEFT_FORWARD, LEFT_BACK},
    //                       {LEFT_BACK, LEFT_FORWARD},
    //                       {RIGHT_FORWARD, RIGHT_BACK},
    //                       {RIGHT_BACK, RIGHT_FORWARD}};

    test_start_game(g);

    // init the graphic cache
    GraphicCache *cache = initCache();

    // end debug

    if (cache->statut != EXIT_SUCCESS)
    {
        return quit(g, cache);
    }

    cache->last_time = SDL_GetTicks();

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
            update(g, cache);
        }
    }
    return quit(g, cache);
}