#include "input_command.h"
#include <stdio.h>

//protocol applied at the end of every turn
void endTurnGraphics(Game* g, GraphicCache* cache){
    //the turn changing will be donne at the end of the
    //change timer from a graphic point of view
    alert(cache, g->indCheck, CHANGE_TICKS);
}

//Click on a pawn to select it
void onLMBDown(Game* g, GraphicCache* cache){
    SDL_Event event = cache->event;
    int x_mouse = event.button.x;
    int y_mouse = event.button.y;

    // printf("pt\n");
    if (g->is_white){
        y_mouse = LG_WINDOW - y_mouse;
    }

    // printf("%d, %d\n", x_mouse, y_mouse);
    int lig = y_mouse / LG_CASE;
    int col = x_mouse / LG_CASE;
    fflush(stdout);
    Case* selectedCase = &(g->damier[lig][col]);

    //si on clique sur un pion allie, selectionne ledit pion
    if (!freeCase(*selectedCase) && selectedCase->pawn_color == g->is_white) {
        g->ind_move = selectedCase->ind_pawn;
    } //si on selectionne une dame et qu'on clique sur une case vide, effectue le deplacement si possible
    else if (g->ind_move > NEUTRAL_IND && g -> allPawns[g->is_white][g->ind_move].queen) {
        Coord finalPos = {lig, col};
        Coord* placeHolder; //cet argument est inutile en dehors de queenCanMoveOrEat
        if (queenCanMoveOrEat(g, g->is_white, g->ind_move, finalPos, placeHolder) == 2) {
            alert(cache, IND_PB, ERROR_TICKS);
        }
        else {
            queenMoveOrEat(g, g->is_white, g->ind_move, finalPos);
            endTurnGraphics(g, cache);
        }
    }

    //logs
    if (g->ind_move == NEUTRAL_IND) {
        printf("No pawn selected");
    }
    else {
        pawn* selectedPawn = &(g->allPawns[g->is_white][g->ind_move]);
        printf("Selected pawn : lig %d, col %d", selectedPawn->lig, selectedPawn->col);
    }
    printf("\n");
}

//moves the pawn after checking the move is valid
void checkPawnMove(Game* g, GraphicCache* cache, bool left){
    int ind = g->ind_move; //selected pawn
    bool is_white = g->is_white;

    if (ind == NEUTRAL_IND || !g->allPawns[is_white][ind].alive || g->allPawns[is_white][ind].queen || !canMove(g, g->is_white, g->ind_move, left)) {
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