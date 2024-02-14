#ifndef MOVE_LISTING
#define MOVE_LISTING

#include "../../fundamental_functions/game_functions/Source/source_header.h"

enum moveType{pawnMoveType, promotionType, pawnMoveBackType, biDeplType, queenDeplType, rafleType, lienAmitieType, lienEnnemitieType};

typedef struct Move{
    enum moveType type;

    int manipulatedPawn;
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