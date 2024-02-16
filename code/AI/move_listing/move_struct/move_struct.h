#ifndef MOVE_STRUCT
#define MOVE_STRUCT

#include "../../../fundamental_functions/coord/coord.h"
#include "../../../rafle/calc/path_tree.h"
#include "../../../rafle/rafle_choice/path_struct.h"

enum moveType{pawnMoveType, promotionType, pawnMoveBackType, biDeplType, queenDeplType, rafleType, lienAmitieType, lienEnnemitieType};

typedef struct Move{
    enum moveType type;
    int manipulatedPawn;

    //wether or not the movement of the pawn
    //is made to the left, used in moving and
    //moving back
    bool left;

    queen_move_t coords;

    //selected pawn on the other side for
    //friendship/hate creation
    int lig, col;

    //path and tree corresponding to a rafle
    Path* rafle;
    PathTree* rafleTree;
}Move;

typedef struct MoveTab{
    int size;
    Move* tab;
} MoveTab;

#endif