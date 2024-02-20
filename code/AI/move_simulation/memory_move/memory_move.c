#include "memory_move.h"

memory_move_t *initMemMove(int indMovePawn) {
    memory_move_t *mem = malloc(sizeof(memory_move_t));
    mem->chainy = NULL;
    mem->load_cloud_other = ccreate_list();
    mem->survivor = malloc(sizeof(ind_pba_t));
    mem->indMovePawn = indMovePawn;
    mem->issues = NULL;
    mem->lenghtIssues=0;
    return mem;
}

void freeMemMove(memory_move_t *mem) {
    // chainy est déjà libérée
    if (mem->issues != NULL) free(mem->issues);
    free(mem->load_cloud_other);
    free(mem->survivor);
    free(mem);
}