#include "quantum_functions.h"
#include "../fundamental_functions/game_functions_draughts.h"


int promotion(Game game, bool is_white, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    int choice = rand() % 3;
    if (choice == 1) game.allPawns[is_white][ind].queen = true;
    else if (choice == 2) {
        game.allPawns[is_white][ind].alive=false;
        // Give birth to the ennemy pawn
        int new_ind = game.nb_pawns[!is_white];
        game.allPawns[is_white][new_ind].alive=true;
        game.allPawns[is_white][new_ind].col=game.allPawns[is_white][ind].col;
        game.allPawns[is_white][new_ind].lig=game.allPawns[is_white][ind].lig;
        game.allPawns[is_white][new_ind].color=!is_white;
        game.allPawns[is_white][new_ind].queen=false;
        game.nb_pawns[!is_white]++;
        // Just need to increment, we have enough place (only NB_PAWNS pawns to promote)
    }
}

