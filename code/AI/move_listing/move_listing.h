#ifndef MOVE_LISTING
#define MOVE_LISTING

#include <stdlib.h>
#include "../../fundamental_functions/game_functions/Source/source_header.h"
#include "../../fundamental_functions/game_functions/Logic_and_calculus/logic_functions.h"
#include "move_max.h"

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
}Move;

typedef struct MoveTab{
    int size;
    Move* tab;
} MoveTab;

MoveTab* listMoves(Game* g);
#endif