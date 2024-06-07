#include "../fundamental_functions/game_functions/access_functions/access_functions.h"
#include "../structures_fondamentales/fgame_struct.h"
#include "../structures_fondamentales/fgraphic_struct.h"
#include "../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include "../AI/ai_lists/ai_struct.h"
#include "../AI/ai_lists/analyse/heuristiques.h"
#include "../AI/ai_lists/ecrasement/ecrasement.h"
#include <stdlib.h>

void intChainShuffle(int_chain* l){
    int n = taille_list(l);
    int tmp, j;
    for (int i = 0; i < n; i++) {
        tmp = get(l, i);
        j = rand() % (i + 1);
        seti(l, i, get(l, j));
        seti(l, j, tmp);
    }
}

void moveBackListingTest(Game* g, GraphicCache* cache){
    int nbPawnsToKill = g->nb_pawns[true] - 5;
    for (int i = 0; i < nbPawnsToKill; i++) {
        killPawnByInd(g, true, 0);
    }
    change_pawn_place(g, 0, true, 0, 0);
    change_pawn_place(g, 1, true, 1, 1);
    change_pawn_place(g, 2, true, 1, 3);
    change_pawn_place(g, 3, true, 2, 2);
    change_pawn_place(g, 4, true, 2, 4);

    for (int i = 0; i < 5; i++) {
        push(g->inds_move_back, i);
    }
    intChainShuffle(g->inds_move_back);
}

void alphaBetaPrunningTest(Game* g){
    //test de l'elagage alpha beta avec
    //ll'heuristique triviale ainsi que
    //l'ecrasement arctangeante
    AI* whitePlayer = malloc(sizeof(AI));
    AI* blackPlayer = malloc(sizeof(AI));

    int testDepth = 5;
    double (*testAnalyse)(Game*) = heuristique_miam_trivial;
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

void rafleTest(Game* g){
    int nbBlacksToKill = NB_PAWNS - 4;
    int nbWhitesToKill = NB_PAWNS - 1;

    for (int i = 0; i < nbBlacksToKill; i++) {
        killPawnByInd(g, false, 0);
    }
    for (int i = 0; i < nbWhitesToKill; i++) {
        killPawnByInd(g, true, 0);
    }

    change_pawn_place(g, 0, true, 0, 0);
    change_pawn_place(g, 0, false, 1, 1);
    change_pawn_place(g, 1, false, 1, 3);
    change_pawn_place(g, 2, false, 1, 5);
    change_pawn_place(g, 3, false, 3, 5);
}

void queenRafleTest(Game* g){
    int nbBlacksToKill = NB_PAWNS - 5;
    int nbWhitesToKill = NB_PAWNS - 1;

    for (int i = 0; i < nbBlacksToKill; i++) {
        killPawnByInd(g, false, 0);
    }
    for (int i = 0; i < nbWhitesToKill; i++) {
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