#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "coord/coord.h"
// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE (NB_CASE_LG * NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)

#define NEUTRAL_IND -1
#define IND_CHANGE_ALLOWED -2
#define IND_PB -3
#define IND_NOTHING_HAPPENED -4
#define IND_BAD_CHOICE -5
#define IND_GLORY_QUEEN -6

#define VOID_INDEX -1

#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3

// #define MY_GUARD 1
// // En evaluant dans les files incluant ce file, on évite la double inclusion

// Game structure

typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
} Case;


typedef struct
{
    int lig, col, ffriend, ennemy;
    /* friend: Lien d'amitie avec un autre pion, -1 if no friend */
    bool alive, color, queen;
} pawn;



typedef struct
{
    Case damier[NB_CASE_LG][NB_CASE_LG];
    pawn allPawns[2][2 * NB_PAWNS];
    // In the impossible but theorical case where for instance whites promote all their pawns and unfortunately they all
    // become blacks, bad choice :(
    int nb_pawns[2];
    /* Access with is_white
    Keep in memory an overapproximation of the nb of pawns, but like for merge to union_find, don't decrease the value.
    Indeed, we need a free indice to create a new ennemy pawn, not necessary the first free indice */
    int ind_move, ind_move_back;
    bool is_white;

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

Pour deplacer une dame, selectionne le dame (clic gauche) puis selectionne la case ou tu veux que la dame aille (clic gauche)
Pour qu'une dame mange une piece, selectionne la dame puis selectionne (clic gauche) une case derriere la piece que tu manges
*/

//error handling
void assertAndLog(bool condition, char* message);
    //Fait planter le programme en affichant un message d'erreur
    //si la condition n'est pas respectee

//Logic functions
bool freeCase(Case c);
int NON(int b);
bool becomeDame(pawn p, pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG]);
/* Couronne un pion en dame et s'il avait un ennemi, celui ci meurt */
bool inGame(int lig, int col);
void popPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j); /* kill the pawn and put its values as 
default values */
void pawn_default_value(pawn pawns[], int ind, bool init_is_white); /* Initialize pawn with default values */
void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col);
int dir(int a);
bool outOfBounds(int i, int j);
bool eatingIsOutOfBounds(int i, int j, int add0, int add1);
int getCodeFromDirs(int horizontalDir, int verticalDir);
void getDirsFromCode(int c, int* di, int* dj);

//Aux functions
//void delete_liste(Liste *liste);
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
// entree : un tableau de pions pawns, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
// des entiers add0 et add1 qui indiquent la direction dans laquelle manger
// sortie : booleen indiquant si le pion dans la direction indique peut etre mange
int changeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int nonLoggingChangeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);

//print functions
void pawnMove(Game *g, bool gauche);
void eatPawn(Game *g);

//board functions
void queenDepl(int col, int lig, Game *g);

// Debug functions
void error();
/* print check in the terminal and flush it */
void print_pawns(Game *g); // Affiche les caracteristiques des Pions
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG]); // Affiche ligne, colonne et coordonnees des cases