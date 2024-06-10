#include "memory_move.h"

pawn_archive_t init_cloud_queen() {
    pawn_archive_t res = {.coord = coord_init(), .had_become_queen = false, .old_pba = -1};
    return res;
}

memory_move_t *initMemMove(int indMovePawn, moveType type)
{
    memory_move_t *mem = malloc(sizeof(memory_move_t));
    mem->type = type;
    mem->chainy = NULL;
    mem->load_cloud_same_color = ccreate_list();
    mem->load_cloud_other = ccreate_list();
    mem->issues = NULL;
    mem->lenghtIssues = 1;
    mem->is_deter = true;
    mem->col = -1;
    mem->lig = -1;
    mem->indMovePawn = indMovePawn;
    mem->ghost_pawn_created_bidepl.b = false;
    mem->ghost_pawn_created_bidepl.c = coord_init();
    mem->pos_potential_foe_from_prom = coord_init();
    mem->init_coord_dame_or_rafle = coord_init();
    mem->left = false;
    mem->move_back_left_or_right = NULL;
    mem->friends_which_move_back = NULL;
    mem->prom_need_break_cloud = false;
    mem->is_white = false;
    mem->movePawn = init_cloud_queen();
    mem->pawnCloudOtherColor = init_cloud_queen();
    mem->pawnCloudSameColor = init_cloud_queen();
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
    cfree(mem->load_cloud_same_color);
    cfree(mem->load_cloud_other);
    free(mem);
}