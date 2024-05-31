#ifndef GAME_FUNCTIONS_DRAUGHTS
#define GAME_FUNCTIONS_DRAUGHTS

#include "../../coord/little_linked_list.h"
#include "../../../rafle/calc/path_tree.h"
#include "../../../rafle/rafle_choice/path_struct.h"



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
#define PAWNMOVEBACK 8

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