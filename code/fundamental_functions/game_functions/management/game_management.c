#include "game_management.h"

/*







   GAME FUNCTION MANAGEMENT






*/

bool endTurnGameManagementNGE(Game *g, int indMovedPawn, int indCheck, bool doMoveBack)
{
    // Appeler toujours apres endTurnGameManagement lorsqu'il n'y a plus d'appel recursif sur les coups à faire
    bool become_a_queen = false;
    bool iw = g->is_white;

    if (!doMoveBack)
    {
        if (indCheck != IND_BAD_CHOICE)
        {
            if (canBePromoted(g, iw, indMovedPawn))
            {
                promote(g, iw, indMovedPawn);
                become_a_queen = true;
            } // A verif
        }
        else
        {
            if (canBePromoted(g, !iw, indMovedPawn))
            // Le pion devenu noir peut très bien devenir une dame
            {
                promote(g, !iw, indMovedPawn);
                become_a_queen = true;
            }
        }
        // g->is_white = !g->is_white;

        if (g->currentTree != emptyTree)
        {
            PathTree *m = g->currentTree;
            g->currentTree = NULL;
            pathTreeFree(m);
        }
    }
    return become_a_queen;
}

void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack)
{
    if (!doMoveBack)
    {
        if (indCheck != IND_BAD_CHOICE)
        {

            if (canBePromoted(g, is_white, indMovedPawn))
            {
                promote(g, is_white, indMovedPawn);
            }
            if (canStormBreaks(g, indMovedPawn, is_white))
            {
                indMovedPawn = AleatStormBreaks(g, is_white); // indMovedPawn peut devenir dame ou tuer un autre
                // nuage
                endTurnGameManagementNGE(g, indMovedPawn, IND_NORMAL, false);
            }
            if (canStormBreaksForTheOthers(g, indMovedPawn, is_white))
            {
                int indEnn = AleatStormBreaks(g, !is_white); // Idem indEnn peut devenir dame
                endTurnGameManagementNGE(g, indEnn, IND_NORMAL, false);
            }
        }
        else
        {
            if (canBePromoted(g, !is_white, indMovedPawn))
            // Le pion devenu noir peut très bien devenir une dame
            {
                promote(g, !is_white, indMovedPawn);
            }
            if (canStormBreaksForTheOthers(g, indMovedPawn, !is_white))
            {
                int indEnn = AleatStormBreaks(g, is_white); // Et il peut faire éclater un nuage de pions
                // En éclatant, indEnn peut devenir une dame
                endTurnGameManagementNGE(g, indEnn, IND_NORMAL, false);
            }
        }

        g->is_white = !g->is_white;
    }
    else
    {
        if (canStormBreaksForTheOthers(g, indMovedPawn, is_white))
        {
            int indEnn = AleatStormBreaks(g, !is_white); // Et il peut faire éclater un nuage de pions
            // En éclatant, indEnn peut devenir une dame
            endTurnGameManagementNGE(g, indEnn, IND_NORMAL, false);
        }
    }
    g->ind_move = NEUTRAL_IND;
    g->indCheck = indCheck;
    if (g->currentTree != emptyTree)
    {
        PathTree *m = g->currentTree;
        g->currentTree = NULL;
        pathTreeFree(m);
    }
}