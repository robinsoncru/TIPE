#include "main.h"
#include "fundamental_functions/game_functions/aux_game_functions/aux_functions.h"

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
    // // Coord c2 = coord_from_ind(g, 1, true);
    // for (int i = 0; i < 4; i++)
    // {
    //     // promote(g, true, i);
    //     // promote(g, false, i);
    //     change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 5, get_pawn_value(g, false, i, COL) + 2*i+1);

    //    lienAmitieNGE(c1.i, c1.j, i, false, g);
    //     promote(g, false, i);
    // }
    // for (int i = NB_PAWNS - 1; i > 3; i--)
    // {
    //     killPawnByInd(g, true, i);
    //     killPawnByInd(g, false, i);
    // }

    // for (int i = 0; i <= 3; i++)
    // {
    //     change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 5, get_pawn_value(g, false, i, COL) + 2 * i + 1);
    //     change_pawn_place(g, i, true, get_pawn_value(g, true, i, LIG) + 5, get_pawn_value(g, true, i, COL) + 2 * i + 1);
    // }

    // killPawnByInd(g, true, 3);

    // for (int i = 2; i > 0; i--)
    // {
    //     change_pawn_place(g, i, true, 4, get_pawn_value(g, true, i, COL) + 2*i + 2);
    //     biDepl(g, i, true);
    // }

    // change_pawn_place(g, 0, true, get_pawn_value(g, true, 0, LIG) + 1, NB_CASE_LG - 5);
    // promote(g, true, 0);
    // // change_pawn_place(g, 0, false, get_pawn_value(g, false, 0, LIG) - 1, NB_CASE_LG-3);

    // change_pawn_place(g, 1, true, get_pawn_value(g, true, 1, LIG) + 1, NB_CASE_LG-3);
    // change_pawn_place(g, 1, false, get_pawn_value(g, false, 1, LIG) - 1, NB_CASE_LG-3);

    // // Configuration triangle
    // change_pawn_place(g->allPawns[1], g->damier, 0, 6, 6);
    // change_pawn_place(g->allPawns[0], g->damier, 2, 5, 5);
    // change_pawn_place(g->allPawns[0], g->damier, 0, 5, 3);
    // // change_pawn_place(g->allPawns[0], g->damier, 1, 3, 5);
    // change_pawn_place(g->allPawns[0], g->damier, 1, 3, 3);
    // change_pawn_place(g->allPawns[0], g->damier, 0, 3, 1);

    // Configuration move back + cloud
    // change_pawn_place(g->allPawns[1], g->damier, 0, 6, 6);
    // change_pawn_place(g->allPawns[1], g->damier, 1, 4, 4);
    // change_pawn_place(g->allPawns[0], g->damier, 1, 3, 5);
    // change_pawn_place(g->allPawns[0], g->damier, 2, 1, 1);

    // configuration en lignes noires
    // pour tester les rafles
    /*
    for (int i = NB_PAWNS - 1; i >= 4; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= 6; i--) {
        killPawnByInd(g, false, i);
    }

    change_pawn_place(g, 0, true, 0, 2);

    change_pawn_place(g, 0, false, 1, 1);
    change_pawn_place(g, 1, false, 3, 1);
    change_pawn_place(g, 2, false, 5, 1);
    change_pawn_place(g, 3, false, 5, 3);
    change_pawn_place(g, 4, false, 3, 3);
    change_pawn_place(g, 5, false, 1, 3);*/
    // fin de config

    // configuration multirafles
    // change_pawn_place(g, 0, true, 0, 0);
    // change_pawn_place(g, 1, true, 0, 2);
    // change_pawn_place(g, 2, true, 0, 4);
    // change_pawn_place(g, 3, true, 0, 6);
    // change_pawn_place(g, 4, true, 2, 0);
    // change_pawn_place(g, 5, true, 2, 2);
    // change_pawn_place(g, 6, true, 2, 4);
    // change_pawn_place(g, 7, true, 2, 6);

    // change_pawn_place(g, 0, false, 3, 1);
    // change_pawn_place(g, 1, false, 3, 3);
    // change_pawn_place(g, 2, false, 3, 5);
    // change_pawn_place(g, 3, false, 3, 7);
    // change_pawn_place(g, 4, false, 5, 1);
    // change_pawn_place(g, 5, false, 5, 3);
    // change_pawn_place(g, 6, false, 5, 5);
    // change_pawn_place(g, 7, false, 5, 7);
    // fin de config

    // print_pawns(g, true);
    print_pawns(g, false);
    // print_liensAmitie(g);
    // print_state_game(g, QUEEN);
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
            nb_coups = update(g, cache, nb_coups, l_coups, l_depl);
        }
    }
    return quit(g, cache);
}