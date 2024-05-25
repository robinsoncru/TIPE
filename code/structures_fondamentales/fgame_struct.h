#ifndef FGAME
#define FGAME

#include "fliste_chaine_struct.h"
#include "fpath_struct.h"



#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h> // Pour le temps de sommeil



// Game structure
typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
} Case;

typedef struct
{
    int lig, col, friendly, ennemy;
    // rename friendly to friendly
    /* friend: Lien d'amitie avec un autre pion, -1 if no friend */
    bool alive, color, queen;
    int pba; // Appartenance au nuage de pion se traduit par pba != 1 et plus pba est grand, plus sa probabilite de presence est faible
} pawn;

typedef struct
{
    Case **damier;
    pawn *allPawns[2];
    // In the impossible but theorical case where for instance whites promote all their pawns and unfortunately they all
    // become blacks, bad choice :( 
    int nb_pawns[2];
    /* Access with is_white
    Keep in memory an overapproximation of the nb of pawns, but like for merge to union_find, don't decrease the value.
    Indeed, we need a free indice to create a new ennemy pawn, not necessary the first free indice */
    int ind_move, indCheck;
    int_chain *inds_move_back;
    int players[2]; //the first is white, the second black, useful to be able to identify AIs
    bool is_white;
    int_chain *cloud[2];
    int lengthCloud[2];
    int nbFriendNoQueen[2];
    int nbFoe[2];
    int nbQueenWithoutFriend[2];
    int nbQueenWithFriend[2];

    PathTree* currentTree;
    Path* currentRafle;
} Game;



#endif