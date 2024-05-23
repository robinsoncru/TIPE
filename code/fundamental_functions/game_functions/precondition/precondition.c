#include "precondition.h"

bool freeCase(Case c)
{
    return c.ind_pawn == -1;
}

bool isValidIndexInGame(Game *g, int ind, bool color) {
    return 0 <= ind && ind < g->nb_pawns[color]  && g->allPawns[color][ind].alive;
}

bool isValidIndex(int ind) {
    return 0 <= ind && ind < 2*NB_PAWNS;
}

bool isPawnValid(Game *g)
{
    // Do the basic checks: ind_move is a valid index and the pawn selected is alive
    return isValidIndexInGame(g, g->ind_move, g->is_white);
}

// Checks if the (i, j) position is out of bounds
bool outOfBounds(int i, int j)
{
    return i < 0 || i >= NB_CASE_LG || j < 0 || j >= NB_CASE_LG;
}

// Checks if a case is in the game
bool inGame(int lig, int col)
{
    return !outOfBounds(lig, col);
}

// Checks if eating from position (i, j) in the (add0, add1) direction leads to an out of bounds position
bool eatingIsOutOfBounds(int i, int j, int add0, int add1)
{
    return outOfBounds(i + 2 * add0, j + 2 * add1);
}

bool isInCloud(Game *g, bool color, int ind)
{
    return (g->allPawns[color][ind].pba > 1);
}

bool enoughPawns(Game *g, bool color) {
    return g->nb_pawns[color] > 0;
}