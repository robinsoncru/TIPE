#include "logic_functions.h"

/*







   LOGIC FUNCTIONS






*/

// checks if a pawn is allowed to move forward
// in the indicated direction
bool canMove(Game *g, bool is_white, int ind, bool left)
{
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {get_pawn_value(g, is_white, ind, LIG) + di,
                      get_pawn_value(g, is_white, ind, COL) + dj};
    bool posInGame = inGame(finalPos.i, finalPos.j);
    return posInGame && freeCase(get_case_damier(g, finalPos.i, finalPos.j));
}

// un ghost pawn ne peut pas devenir une dame
bool canBePromoted(Game *g, bool is_white, int ind)
{
    pawn p = get_pawn(g, is_white, ind);
    int border = is_white ? (NB_CASE_LG - 1) : 0;
    bool isOnBorder = p.lig == border;
    bool ennemyIsDead = p.ennemy != NEUTRAL_IND && !int_to_bool(get_pawn_value(g, !is_white, p.ennemy, ALIVE));
    return !p.queen && (isOnBorder || ennemyIsDead) && p.pba == 1;
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
        if (!freeCase(get_case_damier(g, testedPos.i, testedPos.j)))
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
        get_pawn_value(g, is_white, ind, LIG),
        get_pawn_value(g, is_white, ind, COL)};
    bool isInGame = inGame(finalPos.i, finalPos.j);
    bool isOnDiag = abs(finalPos.i - initPos.i) == abs(finalPos.j - initPos.j);
    return isInGame && isOnDiag && diagIsFree(g, initPos, finalPos);
}

// For eatPawn and rafle calculation
bool canEat(Game *g, bool eatingColor, int ind, int i, int j, int add0, int add1)
{
    assert(isValidIndexInGame(g, ind, eatingColor));
    assert(inGame(i, j));
    assert(inGame(i + 2 * add0, j + 2 * add1));
    return (freeCase(get_case_damier(g, i + 2 * add0, j + 2 * add1)) &&
            !freeCase(get_case_damier(g, i + add0, j + add1)) &&
            get_case_damier(g, i + add0, j + add1).pawn_color != eatingColor &&
            int_to_bool(
                get_pawn_value(g, get_case_damier(g, i + add0, j + add1).pawn_color, get_case_damier(g, i + add0, j + add1).ind_pawn,
                               ALIVE)));
}

bool moveBackAvailable(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i > VOID_INDEX && g->coordForMoveBack.j > VOID_INDEX;
}

bool canBeFriend(Game *g, int ind, bool color, Case c)
{
    // Check if a pawn and the pawn on the case c can be friend
    pawn p = get_pawn(g, color, ind);
    if (freeCase(c))
        return false;
    pawn ap = get_pawn(g, !color, c.ind_pawn);
    return c.pawn_color != p.color && ap.friendly == -1 && p.friendly == -1 && p.ennemy == -1 && ap.ennemy == -1 && !p.queen && !ap.queen;
}

bool canBeEnnemy(Game *g, int ind, bool color, Case c)
{
    return canBeFriend(g, ind, color, c);
}

bool canMoveBack(Game *g, bool is_white, int ind, bool left)
{
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {get_pawn_value(g, is_white, ind, LIG) - di,
                      get_pawn_value(g, is_white, ind, COL) + dj};
    if (inGame(finalPos.i, finalPos.j))
    {
        return freeCase(get_case_damier(g, finalPos.i, finalPos.j));
    }
    else
        return false;
}

bool canBiDepl(Game *g, int ind, bool color)
{
    /* On ne peut pas bidepl une dame. Si le pion est ami ou ennemi, le pion clone a gauche conservera cette amitie ou ennemitie */
    return isPawnValid(g) && !int_to_bool(get_pawn_value(g, color, ind, QUEEN)) && canMove(g, color, ind, true) &&
           canMove(g, color, ind, false);
}

bool needPutMoveBack(Game *g)
{
    return g->ind_move_back > -1 && g->coordForMoveBack.i == IND_LISTENING_MOVE_BACK && g->coordForMoveBack.j == IND_LISTENING_MOVE_BACK;
}

bool caseIsAccessible(Game *g, bool is_white, int i, int j)
{
    return !outOfBounds(i, j) && get_case_damier(g, i, j).ind_pawn == VOID_INDEX;
}

bool canPromotion(Game *g)
{
    return isPawnValid(g) && !isInCloud(g, g->is_white, g->ind_move);
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
        if (inGame(i + di, j + dj))
        {
            c = get_case_damier(g, i + di, j + dj);
            if (!freeCase(c) && c.pawn_color != color && !isInCloud(g, !color, c.ind_pawn))
            {
                return true;
            }
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
            c = get_case_damier(g, i + di, j + dj);
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