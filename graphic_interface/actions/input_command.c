#include "input_command.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>

//Click on a pawn to select it
// void onLMBDown(Game* g, GraphicCache* cache){
//     SDL_Event event = cache->event;
//     if (g->ind_move > NEUTRAL_IND && g -> allPawns[g->is_white][g->ind_move].queen) {
//         //need to debug
//         queenDepl(event.button.x / LG_CASE, event.button.y / LG_CASE, g);
//     }
//     else {
//         selectPawn(g, event.button.x, event.button.y);
//     }

//     //logs
//     if (g->ind_move == NEUTRAL_IND) {
//         printf("\nNo pawn selected\n");
//     }
// }

//protocol applied at the end of every turn
void endTurnGraphics(Game* g, GraphicCache* cache){
    //the turn changing will be donne at the end of the
    //change timer from a graphic point of view
    alert(cache, g->indCheck, CHANGE_TICKS);
}

//moves the pawn after checking the move is valid
void checkPawnMove(Game* g, GraphicCache* cache, bool left){
    int ind = g->ind_move; //selected pawn
    bool is_white = g->is_white;

    bool validIndex = ind > -1;
    bool pawnAlive = g->allPawns[is_white][ind].alive; //ha, ha, ha, ha, pawn alive, pawn alive !
    bool isQueen = g->allPawns[is_white][ind].queen;
    bool legalMove = canMove(g, g->is_white, g->ind_move, left);
    if (!validIndex || !pawnAlive || isQueen || !legalMove) {
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else {
        pawnMove(g, g->is_white, ind, left);
        endTurnGraphics(g, cache);
    }
}

//Moves the pawns to the left.
//After verifying it's possible
void onLeftUp(Game* g, GraphicCache* cache){
    checkPawnMove(g, cache, true);
}

void onRightUp(Game* g, GraphicCache* cache){
    checkPawnMove(g, cache, false);
}

void onEscapeUp(Game* g, GraphicCache* cache){
    cache -> is_playing = false;
}

void onAUp(Game* g, GraphicCache* cache);
void onZUp(Game* g, GraphicCache* cache);
void onRUp(Game* g, GraphicCache* cache);