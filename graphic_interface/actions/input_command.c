#include "input_command.h"
#include <SDL2/SDL_events.h>
#include <stdio.h>

// Click on a pawn to select it
void onLMBDown(Game *g, GraphicCache *cache)
{
    SDL_Event event = cache->event;
    if (g->indCheck == NEUTRAL_IND && g->ind_move == -1)
        // Je garde les conditions pour eviter un comportement indefini, je veux que selectPawn ne soit
        // appeler que sous des conditions bien precise
        selectPawn(g, event.button.x, event.button.y);

    else if (g->ind_move > -1 && g->allPawns[g->is_white][g->ind_move].queen)

        // queenDepl(event.button.x / LG_CASE, event.button.y / LG_CASE, g);
        // Attend le update de queenDepl
        ;

    else if (g->ind_move > -1)

        lienAmitie(event.button.x / LG_CASE, event.button.y / LG_CASE, g);

    if (g->ind_move == -1)

        printf("No pawn selected");
}

void onRMBDown(Game *g, GraphicCache *cache)
{
    SDL_Event event = cache->event;
    lienEnnemitie(event.button.x / LG_CASE, event.button.y / LG_CASE, g);
}

// protocol applied at the end of every turn
void endTurnGraphics(Game *g, GraphicCache *cache)
{
    // the turn changing will be donne at the end of the
    // change timer from a graphic point of view
    alert(cache, g->indCheck, CHANGE_TICKS);
}

// moves the pawn after checking the move is valid
void checkPawnMove(Game *g, GraphicCache *cache, bool left)
{
    int ind = g->ind_move; // selected pawn
    bool is_white = g->is_white;

    bool validIndex = ind > -1;
    bool pawnAlive = g->allPawns[is_white][ind].alive; // ha, ha, ha, ha, pawn alive, pawn alive !
    bool isQueen = g->allPawns[is_white][ind].queen;
    bool legalMove = canMove(g, g->is_white, g->ind_move, left);
    if (!validIndex || !pawnAlive || isQueen || !legalMove)
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else
    {
        pawnMove(g, g->is_white, ind, left);
        endTurnGraphics(g, cache);
    }
}

// Moves the pawns to the left.
// After verifying it's possible
void onLeftUp(Game *g, GraphicCache *cache)
{
    /* Move pawn to left, first back if possible, then forward */
    if (g->ind_move_back != -1)
        putPawnMoveBack(g, true);

    /* Si on doit d'abord deplacer vers l'arriere un de nos pion, on met a jour les coordonnees de
     deplacement arriere puis on verifie si elles son possibles. Si c'est le cas, on deplace en arriere
     sinon on deplace un pion en avant (la disjonction de cas est correct) */
    if (!moveBackNotAvailable(g))
        moveBack(g);
    // Don't need to update the cache, just the game
    else
        checkPawnMove(g, cache, true);
}

void onRightUp(Game *g, GraphicCache *cache)
{
    /* Move pawn to right, same */
    if (g->ind_move_back != -1)
        putPawnMoveBack(g, false);

    if (!moveBackNotAvailable(g))
        moveBack(g);
        // same
    else
        checkPawnMove(g, cache, false);
}

void onEscapeUp(Game *g, GraphicCache *cache)
{
    cache->is_playing = false;
}

void onPUP();

void onAUp(Game *g, GraphicCache *cache);
void onZUp(Game *g, GraphicCache *cache);
void onRUp(Game *g, GraphicCache *cache);