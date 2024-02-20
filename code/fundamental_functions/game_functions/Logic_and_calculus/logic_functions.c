#include "logic_functions.h"

/*







   LOGIC FUNCTIONS






*/

bool freeCase(Case c)
{
    return c.ind_pawn == -1;
}

bool basicChecks(Game *g)
{
    // Do the basic checks: ind_move is a valid index and the pawn selected is alive
    return g->ind_move > VOID_INDEX && g->allPawns[g->is_white][g->ind_move].alive;
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

// checks if a pawn is allowed to move forward
// in the indicated direction
bool canMove(Game *g, bool is_white, int ind, bool left)
{
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {g->allPawns[is_white][ind].lig + di,
                      g->allPawns[is_white][ind].col + dj};
    bool posInGame = inGame(finalPos.i, finalPos.j);
    bool wayIsFree = freeCase(g->damier[finalPos.i][finalPos.j]);
    return posInGame && wayIsFree;
}

// un ghost pawn ne peut pas devenir une dame
bool canBePromoted(Game *g, bool is_white, int ind)
{
    pawn *p = &(g->allPawns[is_white][ind]);
    int border = is_white ? (NB_CASE_LG - 1) : 0;
    bool isOnBorder = p->lig == border;
    bool ennemyIsDead = p->ennemy != NEUTRAL_IND && !g->allPawns[!is_white][p->ennemy].alive;
    return !p->queen && (isOnBorder || ennemyIsDead) && p->pba == 1;
}

// On suppose que les coordonnees sont bien sur la meme diagonale
bool diagIsFree(Game *g, Coord initPos, Coord finalPos)
{
    int di = (finalPos.i > initPos.i) ? 1 : -1;
    int dj = (finalPos.j > initPos.j) ? 1 : -1;
    Coord dir = {di, dj};
    int dist = abs(finalPos.i - initPos.i);

    Coord testedPos;
    for (int k = 1; k < dist; k++)
    {
        testedPos = add(initPos, mult(k, dir));
        if (!freeCase(g->damier[testedPos.i][testedPos.j]))
        {
            return false;
        }
    }
    return true;
}

// on suppose que le pion indique est une dame
bool queenCanMove(Game *g, bool is_white, int ind, Coord finalPos)
{
    Coord initPos = {
        g->allPawns[is_white][ind].lig,
        g->allPawns[is_white][ind].col};
    bool isInGame = inGame(finalPos.i, finalPos.j);
    bool isOnDiag = abs(finalPos.i - initPos.i) == abs(finalPos.j - initPos.j);
    return isInGame && isOnDiag && diagIsFree(g, initPos, finalPos);
}

// For eatPawn and rafle calculation
bool canEat(pawn *pawns, pawn *Npawns, Case **damier, int ind, int i, int j, int add0, int add1)
{
    return (freeCase(damier[i + 2 * add0][j + 2 * add1]) && damier[i + add0][j + add1].pawn_color == !pawns[ind].color &&
            !freeCase(damier[i + add0][j + add1]) && Npawns[damier[i + add0][j + add1].ind_pawn].pba == 1);
}

bool moveBackAvailable(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i > VOID_INDEX && g->coordForMoveBack.j > VOID_INDEX;
}

bool canBeFriend(Game *g, int ind, bool color, Case c)
{
    // Check if a pawn and the pawn on the case c can be friend
    pawn p = g->allPawns[color][ind];
    if (freeCase(c))
        return false;
    pawn ap = g->allPawns[!color][c.ind_pawn];
    return c.pawn_color != p.color && ap.friendly == -1 && p.friendly == -1 && p.ennemy == -1 && ap.ennemy == -1;
}

bool canBeEnnemy(Game *g, int ind, bool color, Case c)
{
    return canBeFriend(g, ind, color, c);
}

bool isInCloud(Game *g, bool color, int ind)
{
    return (g->allPawns[color][ind].pba > 1);
}

bool canMoveBack(Game *g, bool is_white, int ind, bool left)
{
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {g->allPawns[is_white][ind].lig - di,
                      g->allPawns[is_white][ind].col + dj};
    if (inGame(finalPos.i, finalPos.j))
    {
        return freeCase(g->damier[finalPos.i][finalPos.j]);
    }
    else
        return false;
}

bool canBiDepl(Game *g, int ind, bool color)
{
    /* On ne peut pas bidepl une dame. Si le pion est ami ou ennemi, le pion clone a gauche conservera cette amitie ou ennemitie */
    return basicChecks(g) && !int_to_bool(get_pawn_value(g, color, ind, QUEEN)) && canMove(g, color, ind, true) &&
           canMove(g, color, ind, false);
}

bool needPutMoveBack(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i == IND_LISTENING_MOVE_BACK && g->coordForMoveBack.j == IND_LISTENING_MOVE_BACK;
}

bool caseIsAccessible(Game *g, bool is_white, int i, int j)
{
    return !outOfBounds(i, j) && g->damier[i][j].ind_pawn == VOID_INDEX;
}

bool canPromotion(Game *g)
{
    return basicChecks(g) && !isInCloud(g, g->is_white, g->ind_move);
}

// Seul un pion plein peut faire eclater le nuage
bool canStormBreaks(Game *g, int ind, int color)
{
    if (!isInCloud(g, color, ind))
        return false;

    int di, dj;
    int i = get_pawn_value(g, color, ind, LIG);
    int j = get_pawn_value(g, color, ind, COL);
    Case c;
    for (int k = 0; k < 4; k++)
    {

        getDirsFromCode(k, &di, &dj);
        c = g->damier[i + di][j + dj];
        if (!freeCase(c) && c.pawn_color != color && !isInCloud(g, !color, c.ind_pawn))
        {
            return true;
        }
    }
    return false;
}

// Seul un pion plein peut faire eclater le nuage
bool canStormBreaksForTheOthers(Game *g, int ind, int color)
{

    if (isInCloud(g, color, ind))
        return false;
    int di, dj;
    int i = get_pawn_value(g, color, ind, LIG);
    int j = get_pawn_value(g, color, ind, COL);
    Case c;
    for (int k = 0; k < 4; k++)
    {

        getDirsFromCode(k, &di, &dj);
        if (!outOfBounds(i + di, j + dj))
        {
            c = g->damier[i + di][j + dj];
            if (!freeCase(c) && c.pawn_color != color && isInCloud(g, !color, c.ind_pawn))
            {
                return true;
            }
        }
    }
    return false;
}

bool has_friend(Game *g, int ind, int color)
{
    return ((get_pawn_value(g, color, ind, FRIENDLY)) != VOID_INDEX);
}