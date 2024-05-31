#ifndef MEMORY_MOVE
#define MEMORY_MOVE

#include "../../move_listing/move_listing.h"
#include "../../../fundamental_functions/affichage/affichage.h"

typedef struct
{
    float pba;
    Coord pos_survivor;
    int choice_promotion;
} issue_t;

typedef struct memory_move_t memory_move_t;

struct memory_move_t
{
    // Les pions sont enregistrés par leurs coordonnées qui sont un moyen sur de les repérer
    bool is_deter;
    moveType type;
    cloud_chain *load_cloud_other; // Cette structure est valide
    int survivor_pba;              /* On ne garde que la proba du survivor qu'on enregistre lors de l'eclatement du
                 nuage */
    Coord pos_potential_foe_from_prom;
    ind_bool_t full_pawn_data;
    Coord init_coord;
    data_chain *chainy;
    int indMovePawn;
    bool colorIndPawn;
    bool left;
    int lig;
    int col;
    issue_t *issues; // Indices acceptés car on applique immédiatement les issues
    int lenghtIssues;
    int had_become_a_queen; // Enregistre une éventuelle promotion de la dame, il est aussi
    // utilisé pour une promotion d'un pion devenu de la couleur opposé juste après une promotion
};

memory_move_t *initMemMove(int indMovePawn, moveType type);
void freeMemMove(memory_move_t *mem);

#endif