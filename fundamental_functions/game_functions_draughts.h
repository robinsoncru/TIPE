#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE (NB_CASE_LG * NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)
#define IND_PB -3
#define IND_CHANGE_ALLOWED -2
#define NEUTRAL_IND -1
#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3
#define VOID_INDEX -1 //index for empty cases

// Game structure

#ifndef PION_ET_PLATEAU
#define PION_ET_PLATEAU
typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
} Case;

typedef struct
{
    int lig, col;
    bool alive, color, queen;
} pawn;
#endif /*PION_ET_PLATEAU*/

/* NOTES :

pawn.alive:
pawn.color: true if white, false if black
Whites start at lig 0
Blacks start at lig NB_CASE_LG - 1
Case.pawn_color: true for white, false for black
Case.ind_pawn: -1 if no pawn
*/


bool freeCase(Case c);
int NON(int b);
bool becomeDame(pawn p);

//void delete_liste(Liste *liste);
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
    //entree : un tableau de pions pawns, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
    //des entiers add0 et add1 qui indiquent la direction dans laquelle manger
    //sortie : booleen indiquant si le pion dans la direction indique peut etre mange
int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int nonLoggingChangeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
void print_pawns(pawn pawns[]);
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);
int pawn_move(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche);
int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);
bool becomeDame(pawn p);