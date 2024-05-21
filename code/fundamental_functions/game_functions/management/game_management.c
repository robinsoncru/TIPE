#include "game_management.h"

/*







   GAME FUNCTION MANAGEMENT






*/

void moveBackGameManagement(Game *g)
{
    // Remet par defaut les differents indices apres un mouvement arriere du pion
    g->ind_move_back = VOID_INDEX;
}

void endTurnGameManagementSimple(Game *g, int pawn_played)
{
    // Appeler toujours apres endTurnGameManagement lorsqu'il n'y a plus d'appel recursif sur les coups à faire
    if (canBePromoted(g, g->is_white, pawn_played))
    {
        promote(g, g->is_white, pawn_played);
    } // A verif

    // g->is_white = !g->is_white;

    if (g->currentTree != emptyTree)
    {
        PathTree *m = g->currentTree;
        g->currentTree = NULL;
        pathTreeFree(m);
    }
}

void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack)
{

    if (indCheck != IND_BAD_CHOICE)
    {

        if (canBePromoted(g, is_white, indMovedPawn))
        {
            promote(g, is_white, indMovedPawn);
        }
        if (canStormBreaks(g, indMovedPawn, is_white))
            AleatStormBreaks(g, is_white);
        else if (canStormBreaksForTheOthers(g, indMovedPawn, is_white))
            AleatStormBreaks(g, !is_white);
    }
    else if (canStormBreaksForTheOthers(g, indMovedPawn, !is_white))
        AleatStormBreaks(g, is_white);

    g->is_white = !g->is_white;
    g->ind_move = NEUTRAL_IND;
    g->indCheck = indCheck;
    if (doMoveBack)
    {
        g->coordForMoveBack.i = IND_LISTENING_MOVE_BACK;
        g->coordForMoveBack.j = IND_LISTENING_MOVE_BACK;
    }
    if (g->currentTree != emptyTree)
    {
        PathTree *m = g->currentTree;
        g->currentTree = NULL;
        pathTreeFree(m);
    }
}

void startTurnGameManagement(Game *g)
{
    moveBackGameManagement(g);
}