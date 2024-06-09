#ifndef FMEMORY
#define FMEMORY

#include "fcoord_struct.h"
#include "fgame_struct.h"
#include "fliste_chaine_struct.h"

typedef struct
{
    int pba; // On stocke le dénominateur
    Coord pos_survivor;
    int pba_promotion;
} issue_t;

typedef struct
{
    bool had_become_queen;
    Coord coord;
    int old_pba; /* On ne garde que la proba du survivor qu'on enregistre lors de l'eclatement du
                       nuage */
} cloud_queen_t;

typedef struct memory_move_t memory_move_t;

struct memory_move_t
{
    // Les pions sont enregistrés par leurs coordonnées qui sont un moyen sur de les repérer
    bool is_deter;
    moveType type;
    cloud_chain *load_cloud_same_color; // Enregistre les pions fantomes de la même couleur que color
    cloud_chain *load_cloud_other;      // Enregistre les pions fantomes ennemi
    Coord pos_potential_foe_from_prom;
    coord_bool_t ghost_pawn_created_bidepl;
    Coord init_coord_dame_or_rafle; // Enregistre la position d'origine lors d'une rafle ou d'une dame déplacé
    data_chain *chainy;
    int indMovePawn;
    bool left;
    int lig;
    int col;
    issue_t *issues;                // Stocke les positions des pions du cloud
    Coord *friends_which_move_back; // Stocke les positions des pions qui doivent reculer puis réavancer
    // lorsqu'on annule le coup, sa taille est contenu dans move_back_left_or_right
    int lenghtIssues;
    int_chain *move_back_left_or_right;
    bool prom_need_break_cloud;
    bool is_white; // Couleur du jeu durant le tour
    cloud_queen_t movePawn;
    /*On enregistre les coordonnées du pion plutot que son indice qui change
     Enregistre une éventuelle promotion de la dame, il est aussi
     utilisé pour une promotion d'un pion devenu de la couleur opposé juste après une promotion */
    cloud_queen_t pawnCloudSameColor;
    cloud_queen_t pawnCloudOtherColor;
    /* Enregistre les déplacements gauche ou droite des pions qui reculent, à opti avec une bool_chain */
    int_chain *inds_move_back;
};

#endif