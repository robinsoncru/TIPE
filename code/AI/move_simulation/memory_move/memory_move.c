#include "memory_move.h"

memory_move_t *initMemMove(int indMovePawn, moveType type) {
    memory_move_t *mem = malloc(sizeof(memory_move_t));
    mem->type = type;
    mem->chainy = NULL;
    mem->load_cloud_other = ccreate_list();
    mem->survivor = malloc(sizeof(ind_pba_t));
    mem->indMovePawn = indMovePawn;
    mem->issues = NULL;
    mem->lenghtIssues=0;
    mem->is_deter = true;
    mem->col = -1;
    mem->lig = -1;
    mem->full_pawn_data.b = false;
    mem->full_pawn_data.ind = VOID_INDEX;
    mem->ind_potential_foe = VOID_INDEX;
    mem->init_coord.i = -1;
    mem->init_coord.j = -1;
    mem->left = false;
    return mem;
}

void freeMemMove(memory_move_t *mem) {
    // chainy est déjà libérée
    if (mem->issues != NULL) free(mem->issues);
    
    cfree(mem->load_cloud_other);
    free(mem->survivor);
    free(mem);
}