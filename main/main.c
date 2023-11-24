#include "main.h"

int quit(Game* g, GraphicCache* cache){
    freeCache(cache);
    free_game(g);
    TTF_Quit();
    SDL_Quit();
    return cache->statut;
}


int main(int argc, char* argv[]){
    //init the game
    Game* g = create_game();

    int allMoves[4][2] = {{LEFT_FORWARD, LEFT_BACK},
                          {LEFT_BACK, LEFT_FORWARD},
                          {RIGHT_FORWARD, RIGHT_BACK},
                          {RIGHT_BACK, RIGHT_FORWARD}};


    // Intitulé 'Mes conneries' très pratique pour promouvoir une dame en qq coups
    // for (int i = 2; i < NB_PAWNS; i++)
    // {
    //     pawn p = g->allPawns[1][i];
    //     g->allPawns[1][i].alive = false;
    //     g->damier[p.lig][p.col].ind_pawn = -1;
    //     p = g->allPawns[0][i];
    //     g->allPawns[0][i].alive = false;
    //     g->damier[p.lig][p.col].ind_pawn = -1;
    // }
    // change_pawn_place(g->allPawns[1], g->damier, 0, 4, 0);
    // change_pawn_place(g->allPawns[1], g->damier, 1, 4, 2);
    
    //init the graphic cache
    GraphicCache* cache = initCache();

    // end debug

    if (cache->statut != EXIT_SUCCESS) {
        return quit(g, cache);
    }

    cache->last_time = SDL_GetTicks();

    //Start the game
    while (cache->is_playing) {
        cache->time_now = SDL_GetTicks();

        if (cache->time_now - cache->last_time > FRAME) {
            cache->last_time = cache->time_now;

            //clear the window
            SDL_RenderClear(cache->draw);

            //display the data in the cache and the game
            display(g, cache);

            //Rafraichit l'ecran
            SDL_RenderPresent(cache->draw);

            //update the data in the cache
            update(g, cache);
        }
    }
    return quit(g, cache);
}