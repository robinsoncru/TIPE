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

    // config multirafle damier 16 2

    // change_pawn_place(g, 0, true, 2, 0);
    // change_pawn_place(g, 1, true, 2, 2);
    // change_pawn_place(g, 2, true, 2, 4);
    // change_pawn_place(g, 3, true, 2, 6);

    // change_pawn_place(g, 0, false, 3, 1);
    // change_pawn_place(g, 1, false, 3, 3);
    // change_pawn_place(g, 2, false, 3, 5);
    // change_pawn_place(g, 3, false, 3, 7);
    // change_pawn_place(g, 4, false, 5, 1);
    // change_pawn_place(g, 5, false, 5, 3);
    // change_pawn_place(g, 6, false, 5, 5);
    // change_pawn_place(g, 7, false, 5, 7);
    // fin config

    // Coord c1 = coord_from_ind(g, 0, true);
    // Coord c2 = coord_from_ind(g, 1, true);
    // for (int i = 0; i < 4; i++)
    // {
    // promote(g, true, i);
    // promote(g, false, i);
    // change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 5, get_pawn_value(g, false, i, COL) + 2*i+1);

    //     // lienAmitieNGE(c1.i, c1.j, i, false, g);
    //     // promote(g, false, i);
    // }
    // for (int i = NB_PAWNS - 1; i > 3; i--)
    // {
    //     killPawnByInd(g, true, i);
    //     killPawnByInd(g, false, i);
    // }
    // // change_pawn_place(g, 0, false, 1, 3);
    // // change_pawn_place(g, 0, true, NB_CASE_LG-3, 3);

    // for (int i = 0; i <= 3; i++)
    // {
    //     // change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 4, get_pawn_value(g, false, i, COL) + 2 * i + 2);
    //     change_pawn_place(g, i, true, get_pawn_value(g, true, i, LIG) + 11, get_pawn_value(g, true, i, COL) + 2 * i + 1);
    // }
    // // killPawnByInd(g, true, 3);

    // for (int i = 3; i > 0; i--)
    // {
    //     change_pawn_place(g, i, true, 12, get_pawn_value(g, true, i, COL)-1);
    //     g->ind_move = i;
    //     // // killPawnByInd(g, true, i);
    //     checkBiDepl(g, cache, true);
    //     g->is_white = true;
    // }

    // change_pawn_place(g, 0, true, get_pawn_value(g, true, 0, LIG), NB_CASE_LG - 7);
    // change_pawn_place(g, 0, false, get_pawn_value(g, false, 0, LIG) - 1, NB_CASE_LG-3);

    // change_pawn_place(g, 1, true, get_pawn_value(g, true, 1, LIG) + 1, NB_CASE_LG-3);
    // change_pawn_place(g, 1, false, get_pawn_value(g, false, 1, LIG) - 1, NB_CASE_LG-3);

    // Config photo diapo
    int endBlanc = 6;
    int endNoir = 6;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }

    change_pawn_place(g, 4, true,  10, 12);
    // promote(g, true, 4);
    for (int i=0; i<4; i++) {
        change_pawn_place(g, i, true, get_pawn_value(g, true, i, LIG)+4, get_pawn_value(g, true, i, COL));
        change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG)-4, get_pawn_value(g, false, i, COL));
        // lienAmitieNGE(12, 2, i, false, g);
    }

    // change_pawn_place(g, 5, true, 12, 6);
    // biDeplNGE(g, true, 4);
    // biDeplNGE(g, true, 6);
    // biDeplNGE(g, true, 1);


    // change_pawn_place(g, 0, false, 6, 8);
    // change_pawn_place(g, 0, true, 2, 8);
    // biDepl(g, 0, true);

    // biDepl(g, 0, true);
    // biDepl(g, 1, true);
    // biDepl(g, 2, true);
    // biDepl(g, 3, true);
    // biDepl(g, 4, true);
    // biDepl(g, 5, true);



    // // change_pawn_place(g, 1, true, 7, 9);
    // // change_pawn_place(g, 2, true, 5, 11);
    // change_pawn_place(g, 3, true, 2, 4);

    // change_pawn_place(g, 0, false, 2, 6);
    // change_pawn_place(g, 1, false, 3, 7);
    // change_pawn_place(g, 2, false, 4, 6);
    // change_pawn_place(g, 3, false, 3, 5);

    // change_pawn_place(g, 3, false, 3, 11);

    // for (int i=0; i<8; i++) {
    //     change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG)+4, get_pawn_value(g, false, i, COL)+8);
    // }

    // killPawnByInd(g, false, 0);
    // killPawnByInd(g, false, 4);
    // killPawnByInd(g, false, 6);
    //     killPawnByInd(g, false, 1);

    // strat miam
    // change_pawn_place(g, 0, true, 8, 10);
    // change_pawn_place(g, 1, true, 7, 9);
    // change_pawn_place(g, 2, true, 5, 11);
    // change_pawn_place(g, 3, true, 3, 11);

    // change_pawn_place(g, 8, false, 2, 0);

    // strat border
    // change_pawn_place(g, 0, true, 12, 2);
    // change_pawn_place(g, 1, true, 5, 3);
    // // change_pawn_place(g, 2, true, 5, 11);
    // // change_pawn_place(g, 3, true, 3, 11);

    // change_pawn_place(g, 2, false, 8, 6);
    // change_pawn_place(g, 5, false, 6, 10);
    // change_pawn_place(g, 3, false, 14, 6);
    // sous config Nuage apres rafle

    // biDepl(g, 9, false);
    // biDepl(g, 11, false);

    // biDepl(g, 13, false);

    // biDepl(g, 15, false);

    // change_pawn_place(g, 16, false, 3, 9);
    // g->is_white = true;

    // fin ss config

    // change_pawn_place(g, 0, true, 3, 1);
    // change_pawn_place(g, 1, true, 2, 2);
    // change_pawn_place(g, 2, true, 1, 5);

    // change_pawn_place(g, 0, false, 5, 5);
    // change_pawn_place(g, 1, false, 4, 6);
    // change_pawn_place(g, 2, false, 5, 7);
    // change_pawn_place(g, 3, false, 3, 5);
    // change_pawn_place(g, 4, false, 4, 4);
    // fin config

    // configuration en lignes noires
    // pour tester les rafles

    // for (int i = NB_PAWNS - 1; i >= 1; i--)
    // {
    //     killPawnByInd(g, true, i);
    // }

    // for (int i = NB_PAWNS - 1; i >= 16; i--) {
    //     killPawnByInd(g, false, i);
    // }

    // change_pawn_place(g, 0, true, 0, 2);

    // change_pawn_place(g, 0, false, 1, 1);
    // change_pawn_place(g, 1, false, 3, 1);
    // change_pawn_place(g, 2, false, 5, 1);
    // change_pawn_place(g, 3, false, 5, 3);
    // change_pawn_place(g, 4, false, 3, 3);
    // change_pawn_place(g, 5, false, 1, 3);

    // biDeplNGE(g, false, 9);
    // biDeplNGE(g, false, 11);
    // biDeplNGE(g, false, 13);

    // change_pawn_place(g, 9, false, 3, 5);

    // change_pawn_place(g, 0, true, 7, 7);

    // // for (int i = 15; i >= 0; i--) {
    // //     lienAmitieNGE(7, 7, i, false, g);
    // // }

    // promote(g, true, 0);
    // fin de config

    // configuration multirafles damier 16
    // change_pawn_place(g, 0, true, 2, 2);
    // // change_pawn_place(g, 1, true, 0, 2);
    // // change_pawn_place(g, 2, true, 0, 4);
    // // change_pawn_place(g, 3, true, 0, 6);
    // // change_pawn_place(g, 4, true, 2, 0);
    // // change_pawn_place(g, 5, true, 2, 2);
    // // change_pawn_place(g, 6, true, 2, 4);
    // // change_pawn_place(g, 7, true, 2, 6);

    // change_pawn_place(g, 0, false, 3, 1);
    // change_pawn_place(g, 1, false, 3, 3);
    // change_pawn_place(g, 2, false, 3, 5);
    // change_pawn_place(g, 3, false, 3, 7);
    // change_pawn_place(g, 4, false, 5, 1);
    // change_pawn_place(g, 5, false, 5, 3);
    // change_pawn_place(g, 6, false, 5, 5);
    // change_pawn_place(g, 7, false, 5, 7);

    // for (int i =8; i<12; i++) {
    //     change_pawn_place(g, i, false, 7, get_pawn_value(g, false, i, COL)+1);
    // }

    // for (int i=12; i<16; i++) {
    //     change_pawn_place(g, i, false, 9, get_pawn_value(g, false, i, COL)-7);
    // }

    // killPawnByInd(g, false, 14);
    //     killPawnByInd(g, false, 13);

    // killPawnByInd(g, false, 10);

    // killPawnByInd(g, false, 9);

    // fin de config

    // sous config enn apres rafle
    // lienEnnemitieNGE(false, 2, 0, 5, g);
    // lienEnnemitieNGE(false, 2, 4, 6, g);

    // lienEnnemitieNGE(false, 2, 6, 8, g);

    // lienAmitieNGE(2, 4, 5, false, g);

    // lienAmitieNGE(2, 0, 6, false, g);

    // g->is_white = true;

    // fin ss config

    // Config move back



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

    // int allMoves[4][2] = {{LEFT_FORWARD, LEFT_BACK},
    //                       {LEFT_BACK, LEFT_FORWARD},
    //                       {RIGHT_FORWARD, RIGHT_BACK},
    //                       {RIGHT_BACK, RIGHT_FORWARD}};

    // init the graphic cache
    GraphicCache *cache = initCache();
    test_start_game(g, cache);
    // moveBackListingTest(g, cache);

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