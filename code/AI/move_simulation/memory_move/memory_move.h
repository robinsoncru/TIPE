#ifndef MEMORY_MOVE
#define MEMORY_MOVE

#include "../../move_listing/move_listing.h"
#include "../../../fundamental_functions/interface/interface.h"

typedef struct {
    float pba;
    int index;
    Coord pos_survivor;
} issue_t;

typedef struct memory_move_t memory_move_t;

struct memory_move_t {
    bool is_deter;
    moveType type;
    cloud_chain *load_cloud_other;
    ind_pba_t *survivor;
    int ind_potential_foe;
    ind_bool_t full_pawn_data;
    Coord init_coord;
    data_chain *chainy;
    int indMovePawn;
    bool left;
    int lig;
    int col;
    issue_t *issues;
    int lenghtIssues;
};

memory_move_t *initMemMove(int indMovePawn, moveType type);
void freeMemMove(memory_move_t *mem);

#endif