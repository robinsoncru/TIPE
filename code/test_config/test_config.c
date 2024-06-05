#include "../fundamental_functions/game_functions/access_functions/access_functions.h"
#include "../structures_fondamentales/fgame_struct.h"
#include "../structures_fondamentales/fgraphic_struct.h"
#include "../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"

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