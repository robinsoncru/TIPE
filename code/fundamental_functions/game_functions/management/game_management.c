#include "game_management.h"

/*







   GAME FUNCTION MANAGEMENT






*/

void endTurnGameManagementNGE(Game *g, int indMovedPawn, int indCheck, bool doMoveBack, memory_move_t *mem)
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
            }
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

        if (g->currentTree != emptyTree)
        {
            PathTree *m = g->currentTree;
            g->currentTree = NULL;
            pathTreeFree(m);
        }
    }
    mem->movePawn.had_become_queen = become_a_queen;
}

void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack)
{
    if (!doMoveBack)
    {
        if (indCheck != IND_BAD_CHOICE)
        {
            handlePawnGhostAndPromotion(g, is_white, indMovedPawn);
        }
        else
        {
            handlePawnGhostAndPromotion(g, !is_white, indMovedPawn);
        }

        g->is_white = !g->is_white;
    }
    else
    {
        handlePawnGhostAndPromotion(g, is_white, indMovedPawn);
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

void enTurnGameManagementForGhosts(Game *g, int ind, int newInd, bool color, int indCheck)
{
    // Ind : pion qui se bidepl
    // newInd : pion crée suite au bidepl
    handlePawnGhostAndPromotion(g, color, ind);
    // ind peut casser le nuage
    if (isValidIndexInGame(g, newInd, color)) {
        // tout comme newInd, donc on vérif les deux
        handlePawnGhostAndPromotion(g, color, newInd);
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

void handlePawnGhostAndPromotion(Game *g, bool color, int ind)
{
    // Gère les rétroactions des nuages et des promotions
    int indJesus;
    while (ind != VOID_INDEX)
    {
        indJesus = VOID_INDEX;
        if (canBePromoted(g, color, ind))
        {
            promote(g, color, ind);
        }
        if (canStormBreaks(g, ind, color))
        {
            indJesus = AleatStormBreaks(g, color);
        }
        else if (canStormBreaksForTheOthers(g, ind, color))
        {
            indJesus = AleatStormBreaks(g, !color);
            color = !color;
        }
        ind = indJesus;
    }
}