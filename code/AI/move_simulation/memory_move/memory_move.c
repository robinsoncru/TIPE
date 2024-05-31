#include "memory_move.h"

memory_move_t *initMemMove(int indMovePawn, moveType type)
{
    memory_move_t *mem = malloc(sizeof(memory_move_t));
    mem->type = type;
    mem->chainy = NULL;
    mem->load_cloud_other = ccreate_list();
    mem->indMovePawn = indMovePawn;
    mem->issues = NULL;
    mem->lenghtIssues = 1;
    mem->is_deter = true;
    mem->col = -1;
    mem->lig = -1;
    mem->full_pawn_data.b = false;
    mem->full_pawn_data.ind = VOID_INDEX;
    mem->pos_potential_foe_from_prom.i = -1;
    mem->pos_potential_foe_from_prom.j = -1;
    mem->init_coord.i = -1;
    mem->init_coord.j = -1;
    mem->left = false;
    mem->had_become_a_queen = false;
    mem->survivor_pba = -1;
    mem->move_back_left_or_right = NULL;
    return mem;
}

void freeMemMove(memory_move_t *mem)
{
    // chainy est déjà libérée
    if (mem->issues != NULL)
        free(mem->issues);
    if (mem->move_back_left_or_right != NULL)
        free(mem->move_back_left_or_right);
    cfree(mem->load_cloud_other);
    free(mem);
}