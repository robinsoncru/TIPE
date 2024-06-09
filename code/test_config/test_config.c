#include "../fundamental_functions/game_functions/access_functions/access_functions.h"
#include "../structures_fondamentales/fgame_struct.h"
#include "../structures_fondamentales/fgraphic_struct.h"
#include "../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include "../AI/ai_lists/ai_struct.h"
#include "../AI/ai_lists/analyse/heuristiques.h"
#include "../AI/ai_lists/ecrasement/ecrasement.h"
#include "test_config.h"
#include <stdlib.h>

void intChainShuffle(int_chain *l)
{
    int n = taille_list(l);
    int tmp, j;
    for (int i = 0; i < n; i++)
    {
        tmp = get(l, i);
        j = rand() % (i + 1);
        seti(l, i, get(l, j));
        seti(l, j, tmp);
    }
}

void moveBackListingTest(Game *g, GraphicCache *cache)
{
    int nbPawnsToKill = g->nb_pawns[true] - 5;
    for (int i = 0; i < nbPawnsToKill; i++)
    {
        killPawnByInd(g, true, 0);
    }
    change_pawn_place(g, 0, true, 0, 0);
    change_pawn_place(g, 1, true, 1, 1);
    change_pawn_place(g, 2, true, 1, 3);
    change_pawn_place(g, 3, true, 2, 2);
    change_pawn_place(g, 4, true, 2, 4);

    for (int i = 0; i < 5; i++)
    {
        push(g->inds_move_back, i);
    }
    intChainShuffle(g->inds_move_back);
}

void alphaBetaPrunningTest(Game *g)
{
    // test de l'elagage alpha beta avec
    // ll'heuristique triviale ainsi que
    // l'ecrasement arctangeante
    AI *whitePlayer = malloc(sizeof(AI));
    AI *blackPlayer = malloc(sizeof(AI));

    int testDepth = 5;
    double (*testAnalyse)(Game *) = heuristique_miam_trivial;
    double (*testEcrasement)(double) = erf;

    whitePlayer->depth = testDepth;
    whitePlayer->analyse = testAnalyse;
    whitePlayer->ecrasement = testEcrasement;

    blackPlayer->depth = testDepth;
    blackPlayer->analyse = testAnalyse;
    blackPlayer->ecrasement = testEcrasement;

    g->players[true] = whitePlayer;
    g->players[false] = blackPlayer;
}

void rafleTest(Game *g)
{
    int nbBlacksToKill = NB_PAWNS - 4;
    int nbWhitesToKill = NB_PAWNS - 1;

    for (int i = 0; i < nbBlacksToKill; i++)
    {
        killPawnByInd(g, false, 0);
    }
    for (int i = 0; i < nbWhitesToKill; i++)
    {
        killPawnByInd(g, true, 0);
    }

    change_pawn_place(g, 0, true, 0, 0);
    change_pawn_place(g, 0, false, 1, 1);
    change_pawn_place(g, 1, false, 1, 3);
    change_pawn_place(g, 2, false, 1, 5);
    change_pawn_place(g, 3, false, 3, 5);
}

void queenRafleTest(Game *g)
{
    int nbBlacksToKill = NB_PAWNS - 5;
    int nbWhitesToKill = NB_PAWNS - 1;

    for (int i = 0; i < nbBlacksToKill; i++)
    {
        killPawnByInd(g, false, 0);
    }
    for (int i = 0; i < nbWhitesToKill; i++)
    {
        killPawnByInd(g, true, 0);
    }

    change_pawn_place(g, 0, true, 0, 0);
    promote(g, true, 0);

    change_pawn_place(g, 0, false, 4, 4);
    change_pawn_place(g, 1, false, 4, 6);
    change_pawn_place(g, 2, false, 4, 8);
    change_pawn_place(g, 3, false, 4, 10);
    change_pawn_place(g, 4, false, 4, 12);
    promote(g, false, 4);
}

void configPhotoDiapo(Game *g)
{
    int endBlanc = 1;
    int endNoir = 3;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }

    change_pawn_place(g, 0, false, 5, 7);
    change_pawn_place(g, 1, false, 5, 5);
    change_pawn_place(g, 2, false, 4, 6);

    change_pawn_place(g, 0, true, 3, 5);
}

void configStratMiam(Game *g)
{

    // change_pawn_place(g, 0, true, 8, 10);
    // change_pawn_place(g, 1, true, 7, 9);
    // change_pawn_place(g, 2, true, 5, 11);
    // change_pawn_place(g, 3, true, 3, 11);

    // change_pawn_place(g, 8, false, 2, 0);
}

void configStratBorder(Game *g)
{

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
}

// Laissé en commentaire, ça compile plus vite

void configNuagePawnMove(Game *g)
{
    int endBlanc = 6;
    int endNoir = 3;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    biDeplNGE(g, true, 2);

    // promote(g, true, 4);
    for (int i = endNoir - 1; i > -1; i--)
    {
        change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 1, get_pawn_value(g, false, i, COL) + 5);
        // lienAmitieNGE(12, 2, i, false, g);
    }
    biDeplNGE(g, false, 0);
    biDeplNGE(g, false, 3);

    change_pawn_place(g, 6, true, 13, 3);
    change_pawn_place(g, 2, true, 15, 1);
    change_pawn_place(g, 0, false, 0, 14);
    change_pawn_place(g, 1, true, 10, 6);
    // biDeplNGE(g, true, 6);
    // biDeplNGE(g, true, 6);
}

void configBiDeplKillNuageAndNuageEnn(Game *g)
{
    int endBlanc = 6;
    int endNoir = 3;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    biDeplNGE(g, true, 2);

    // promote(g, true, 4);
    for (int i = endNoir - 1; i > -1; i--)
    {
        change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 1, get_pawn_value(g, false, i, COL) + 5);
        // lienAmitieNGE(12, 2, i, false, g);
    }

    change_pawn_place(g, 2, true, 14, 4);
    change_pawn_place(g, 4, true, 10, 6);
    // biDeplNGE(g, true, 6);
    // biDeplNGE(g, true, 6);
    biDeplNGE(g, false, 1);
    change_pawn_place(g, 3, false, 14, 2);

    change_pawn_place(g, 1, false, 1, 13);
    biDeplNGE(g, false, 1);
}

void configNuageQueenDepl(Game *g)
{
    int endBlanc = 6;
    int endNoir = 3;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    biDeplNGE(g, true, 2);

    // promote(g, true, 4);
    for (int i = endNoir - 1; i > -1; i--)
    {
        change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 1, get_pawn_value(g, false, i, COL) + 5);
        // lienAmitieNGE(12, 2, i, false, g);
    }
    biDeplNGE(g, false, 0);
    biDeplNGE(g, false, 3);

    change_pawn_place(g, 6, true, 13, 3);
    change_pawn_place(g, 2, true, 15, 1);
    change_pawn_place(g, 0, false, 0, 14);
    change_pawn_place(g, 1, true, 9, 7);
    promote(g, true, 1);
    // biDeplNGE(g, true, 6);
    // biDeplNGE(g, true, 6);
}

void configNuageRafle(Game *g)
{
    int endBlanc = 6;
    int endNoir = 3;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    biDeplNGE(g, true, 2);

    // promote(g, true, 4);
    for (int i = endNoir - 1; i > -1; i--)
    {
        change_pawn_place(g, i, false, get_pawn_value(g, false, i, LIG) - 1, get_pawn_value(g, false, i, COL) + 5);
        // lienAmitieNGE(12, 2, i, false, g);
    }
    biDeplNGE(g, false, 0);
    biDeplNGE(g, false, 3);

    // change_pawn_place(g, 6, true, 13, 3);
    // change_pawn_place(g, 2, true, 15, 1);
    change_pawn_place(g, 0, false, 0, 14);
    change_pawn_place(g, 2, false, 10, 6);
    change_pawn_place(g, 1, true, 9, 7);
    promote(g, true, 1);
    // biDeplNGE(g, true, 6);
    // biDeplNGE(g, true, 6);
}

void configAtome(Game *g)
{
    int endBlanc = 13;
    int endNoir = 1;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    // biDeplNGE(g, true, 2);

    // // promote(g, true, 4);
    change_pawn_place(g, 0, false, 5, 5);

    change_pawn_place(g, 10, true, 8, 6);
    change_pawn_place(g, 11, true, 8, 4);
    change_pawn_place(g, 8, true, 7, 3);

    change_pawn_place(g, 1, true, 4, 2);
    change_pawn_place(g, 2, true, 6, 2);
    change_pawn_place(g, 9, true, 7, 7);

    change_pawn_place(g, 0, true, 6, 8);
    change_pawn_place(g, 3, true, 4, 8);
    change_pawn_place(g, 6, true, 3, 3);

    change_pawn_place(g, 4, true, 2, 6);
    change_pawn_place(g, 5, true, 2, 4);
    change_pawn_place(g, 7, true, 3, 7);

    for (int i = endBlanc - 1; i > -1; i--)
    {
        lienAmitieNGE(5, 5, i, true, g);
    }

    change_pawn_place(g, 12, true, 10, 14);
}

void configGrandAtome(Game *g)
{
    int endBlanc = 16;
    int endNoir = 1;
    for (int i = NB_PAWNS - 1; i >= endBlanc; i--)
    {
        killPawnByInd(g, true, i);
    }

    for (int i = NB_PAWNS - 1; i >= endNoir; i--)
    {
        killPawnByInd(g, false, i);
    }
    // biDeplNGE(g, true, 2);

    // // promote(g, true, 4);
    change_pawn_place(g, 0, false, 6, 6);

    change_pawn_place(g, 10, true, 10, 6);
    change_pawn_place(g, 11, true, 10, 4);
    change_pawn_place(g, 8, true, 10, 8);

    change_pawn_place(g, 12, true, 3, 3);

    change_pawn_place(g, 1, true, 4, 2);
    change_pawn_place(g, 2, true, 6, 2);
    change_pawn_place(g, 9, true, 8, 2);

    change_pawn_place(g, 13, true, 3, 9);

    change_pawn_place(g, 0, true, 6, 10);
    change_pawn_place(g, 3, true, 4, 10);
    change_pawn_place(g, 6, true, 8, 10);

    change_pawn_place(g, 14, true, 9, 3);

    change_pawn_place(g, 4, true, 2, 6);
    change_pawn_place(g, 5, true, 2, 4);
    change_pawn_place(g, 7, true, 2, 8);

    change_pawn_place(g, 15, true, 9, 9);

    for (int i = endBlanc - 1; i > -1; i--)
    {
        lienAmitieNGE(6, 6, i, true, g);
    }

    // change_pawn_place(g, 12, true, 10, 14);
}

// JOUER DES NGE AVEC PLAY_A_MOVE

// if (g->is_white)
//         {
//             printf("Nb coup %d couleur %d", nb_coups, g->is_white);
//             flush();
//             PathTree *t = rafleTreeCalc(g, true, 1);
//             Path *r = lazyRafle(t);
//             Move m = {.backwardPawnMoves = NULL, .left = true, .col = -1, .lig = -1, .manipulatedPawn = 1, .pos_dame = {.i = 9, .j = 7}, .rafle = r, .rafleTree = t, .type = rafleType};
//             memory_move_t *mem = applyDeter(g, m, true);
//             for (int i = 0; i < mem->lenghtIssues; i++)
//             {
//                 applyIssue(g, mem, i);
//                 print_state_game(g);
//                 applyRecipIssue(g, mem, i);
//             }
//             applyRecipDeter(g, mem);
//             print_state_game(g);
//             flush();
//         }