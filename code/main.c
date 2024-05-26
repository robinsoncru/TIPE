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

    // Coord c1 = coord_from_ind(g, 0, true);
    // Coord c2 = coord_from_ind(g, 1, true);
    // for (int i = 0; i < 4; i++)
    // {
    //     // promote(g, true, i);
    //     // promote(g, false, i);
    //     lienAmitiePmetreNGE(c1.i, c1.j, i, false, g);
    // }
    // // for (int i = 2; i < 4; i++)
    // // {
    // //     // promote(g, true, i);
    // //     // promote(g, false, i);
    // //     lienAmitiePmetreNGE(c2.i, c2.j, i, false, g);
    // // }

    // // print_pawns(g, true);
    // // print_pawns(g, false);

    // for (int i = NB_PAWNS-1; i >= 4; i--)
    // {
    //     killPawnByInd(g, false, i);
    // }

    // for (int i=0; i<4; i++) {
    // change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG)-12, get_pawn_value(g, false, i, COL)+i+4);
    // }

    // for (int i = NB_PAWNS -1; i>0; i--) {

    //     killPawnByInd(g, true, i);
    // }

    // // for (int i = 0; i < NB_PAWNS / 2; i++)
    // // {
    // // }

    // // for (int i = 0; i > -1; i--)
    // // {
    // //     killPawnByInd(g, true, i);
    // //     killPawnByInd(g, false, i);
    // // }
    // change_pawn_place(g, 0, true, get_pawn_value(g, true, 0, LIG)+3, NB_CASE_LG-3);
    //     // change_pawn_place(g, 1, true, get_pawn_value(g, true, 1, LIG)+3, NB_CASE_LG-5);

    // // change_pawn_place(g, 0, false, get_pawn_value(g, false, 0, LIG) - 1, NB_CASE_LG-3);

    // // change_pawn_place(g, 1, true, get_pawn_value(g, true, 1, LIG) + 1, NB_CASE_LG-3);
    // // change_pawn_place(g, 1, false, get_pawn_value(g, false, 1, LIG) - 1, NB_CASE_LG-3);


    print_pawns(g, true);
    print_pawns(g, false);
    print_liensAmitie(g);
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
    int nb_coups = 0;
    int l_coups[10] = {4, 4};
    int l_depl[20] = {5, 5};

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
            nb_coups = update(g, cache, nb_coups, l_coups, l_depl);
        }
    }
    return quit(g, cache);
}