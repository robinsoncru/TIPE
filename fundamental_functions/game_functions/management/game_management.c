#include "game_management.h"

/*







   GAME FUNCTION MANAGEMENT






*/

void moveBackGameManagement(Game *g)
{
    // Remet par defaut les differents indices apres un mouvement arriere du pion
    g->ind_move_back = VOID_INDEX;
}

void endTurnGameManagementSimple(Game *g, int pawn_played) {
    // Appeler toujours apres endTurnGameManagement lorsqu'il n'y a plus d'appel recursif sur les coups à faire
    if (canBePromoted(g, g->is_white, pawn_played))
    {
        promote(g, g->is_white, pawn_played);
    } // A verif

    g->is_white = !g->is_white;

    if (g->currentTree != emptyTree) {
        PathTree* m = g->currentTree;
        g->currentTree = NULL;
        pathTreeFree(m);
    }    

}

void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack)
{
    // A penser : pour l'implementation des rafles, il faudrait changer la manniere de manger
    // au lieu de manger chaque pion au fur et a mesure qu'on deplace le mangeur
    // il vaut mieux placer le pion mangeur directement a sa position finale
    // et referencer ses victimes dans une file qu'on va defiler d'un coup
    // l'idee c'est que l'application de potentielles autres regles ne va pas interferer
    // avec l'application des rafles
    //  Les game functions ne renvoient pas que IND_CHANGE_ALLOWED, mais IND_GLORY_QUEEN ...
    if (canBePromoted(g, is_white, indMovedPawn))
    {
        promote(g, is_white, indMovedPawn);
    }


    if (canStormBreaks(g, indMovedPawn, is_white)) AleatStormBreaks(g, is_white);
    else if (canStormBreaksForTheOthers(g, indMovedPawn, is_white)) AleatStormBreaks(g, !is_white);

    g->is_white = !g->is_white;
    g->ind_move = NEUTRAL_IND;
    g->indCheck = indCheck;
    if (doMoveBack)
    {
        g->coordForMoveBack.i = IND_LISTENING_MOVE_BACK;
        g->coordForMoveBack.j = IND_LISTENING_MOVE_BACK;
    }
    if (g->currentTree != emptyTree) {
        PathTree* m = g->currentTree;
        g->currentTree = NULL;
        pathTreeFree(m);
    }
}

void startTurnGameManagement(Game *g)
{
    moveBackGameManagement(g);
}