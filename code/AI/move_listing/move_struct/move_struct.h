#ifndef MOVE_STRUCT
#define MOVE_STRUCT

#include "../../../fundamental_functions/coord/coord.h"
#include "../../../rafle/calc/path_tree.h"
#include "../../../rafle/rafle_choice/path_struct.h"


typedef struct Move
{
    enum moveType type;
    int manipulatedPawn;

    // wether or not the movement of the pawn
    // is made to the left, used in moving and
    // moving back
    bool left;

    //in case of a move back move,
    //list of (ind, bool) couples that indicate
    //which
    backwardMoveTab_t* backwardPawnMoves;

    Coord pos_dame;

    // selected pawn on the other side for
    // friendship/hate creation
    int lig, col;

    // path and tree corresponding to a rafle
    Path *rafle;
    PathTree *rafleTree;
} Move;

typedef struct MoveTab
{
    int size;
    Move *tab;
} MoveTab;

void moveTabFree(MoveTab *moveTab, int startIndex, int endIndex);
void moveTabFreeTrees(MoveTab* moveTab, int startIndex, int endIndex);
void printMove(Move m);
void printMoveTab(MoveTab *t);
void moveFree(Move coup);
#endif