#ifndef GAME_FUNCTIONS_DRAUGHTS
#define GAME_FUNCTIONS_DRAUGHTS

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h> // Pour le temps de sommeil

#include "../../coord/little_linked_list.h"
#include "../../../rafle/calc/path_tree.h"
#include "../../../rafle/rafle_choice/path_struct.h"

// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE (NB_CASE_LG * NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)
#define MAX_NB_PAWNS (2 * NB_PAWNS)

// represente une absence de pion
#define NEUTRAL_IND -1
#define VOID_INDEX (NEUTRAL_IND)
// Maybe we should remove one of these

// check indexes
#define IND_NORMAL -1
#define IND_CHANGE_ALLOWED 0
#define IND_PB 1
#define IND_NOTHING_HAPPENED 2
#define IND_BAD_CHOICE 3
#define IND_GLORY_QUEEN 4
#define IND_LISTENING_MOVE_BACK -2

// directions
#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3

#define RNG_INIT_NBR 14071789

// To modify the pawn values
// 1:alive | 2:ennemy | 3:friendly | 4:queen | 5:lig | 6:col | 7:pba | 8:color
#define ALIVE 1
#define ENNEMY 2
#define FRIENDLY 3
#define QUEEN 4
#define LIG 5
#define COL 6
#define PBA 7
#define COLOR 8

// To modify damier
#define CASE_COLOR 1
#define PAWN_COLOR 2
#define IND_PAWN_ON_CASE 3

// Define index to play a move
#define PAWNMOVELEFT 0
#define PROMOTION 1
#define LIENAMITIE 2
#define LIENDENNEMITIE 3
#define BIDEPL 4
#define PAWNMOVERIGHT 5
#define EATRAFLE 6
#define QUEENDEPL 7
#define PAWNMOVEBACKLEFT 8
#define PAWNMOVEBACKRIGHT 9

//Indexes of the players
#define HUMAN 0
#define RANDOM 1
// #define MY_GUARD 1
// En evaluant dans les files incluant ce file, on évite la double inclusion

// gameplay
#define GDEPL 1
#define GCLOUD 2

// Variables to handle number of each type of pawns
#define NB_QUEEN 0
#define NB_FRIEND 1
#define NB_FOE 2

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
    int ind_move, ind_move_back, indCheck;
    int players[2]; //the first is white, the second black, useful to be able to identify AIs
    bool is_white;
    Coord coordForMoveBack;
    maillon* cloud[2];
    int lengthCloud[2];
    int nbFriendNoQueen[2];
    int nbFoe[2];
    int nbQueenWithoutFriend[2];
    int nbQueenWithFriend[2];

    PathTree* currentTree;
    Path* currentRafle;
} Game;


/* NOTES :

pawn.alive:
pawn.color: true if white, false if black
Whites start at lig 0
Blacks start at lig NB_CASE_LG - 1
Case.pawn_color: true for white, false for black
Case.ind_pawn: -1 if no pawn

Pour deplacer un pion, selectionne le pion (clic gauche) puis fleche gauche pour avancer a gauche et fleche droite pour droite
Pour qu'un pion mange une piece, selectionne le pion qui mange (clic gauche) puis fleche du haut

Pour deplacer une dame, selectionne la dame (clic gauche) puis selectionne la case ou tu veux que la dame aille (clic gauche)
Pour qu'une dame mange une piece, selectionne la dame puis selectionne (clic gauche) une case derriere la piece que tu manges
*/
#endif // GAME_FUNCTIONS_DRAUGHTS