#include "memory_move.h"

memory_move_t *initMemMove(int indMovePawn, moveType type)
{
    memory_move_t *mem = malloc(sizeof(memory_move_t));
    mem->type = type;
    mem->chainy = NULL;
    mem->load_cloud = ccreate_list();
    mem->issues = NULL;
    mem->lenghtIssues = 1;
    mem->is_deter = true;
    mem->col = -1;
    mem->lig = -1;
    mem->indMovePawn = indMovePawn;
    mem->ghost_pawn_created_bidepl.b = false;
    mem->ghost_pawn_created_bidepl.c = coord_init();
    mem->pos_potential_foe_from_prom = coord_init();
    mem->init_coord_dame_rafle = coord_init();
    mem->left = false;
    mem->had_become_a_queen = false;
    mem->survivor_pba = -1;
    mem->move_back_left_or_right = NULL;
    mem->friends_which_move_back = NULL;
    mem->prom_need_break_cloud = false;
    mem->is_white = false;
    mem->coordMovePawn = coord_init();
    return mem;
}

void freeMemMove(memory_move_t *mem)
{
    // chainy est déjà libérée
    if (mem->issues != NULL)
        free(mem->issues);
    if (mem->move_back_left_or_right != NULL)
        freeIntChain(mem->move_back_left_or_right);
    if (mem->friends_which_move_back != NULL)
        free(mem->friends_which_move_back);
    cfree(mem->load_cloud);
    free(mem);
}