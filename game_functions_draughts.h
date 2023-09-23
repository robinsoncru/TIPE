#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE NB_CASE_LG *NB_CASE_LG
#define NB_PAWNS (NB_CASE_LG)
#define IND_PB -3
#define IND_CHANGE_ALLOWED -2
#define NEUTRAL_IND -1
#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3

// Game structure

typedef struct
{
    int occupied;
    int ind_pawn;
    SDL_Rect rect;
    bool color;
} Case;

typedef struct Rafle Rafle;
struct Rafle
{
    int ind_eat;
    Rafle *pt;
};

typedef struct Liste Liste;
struct Liste
{
    Rafle *first;
};

typedef struct
{
    int lig, col, alive;
    bool queen;
} pawn;

/* NOTES :

pawn.alive: 1 for white, 2 for black and 0 for dead
Whites start at lig 0
Blacks start at lig NB_CASE_LG - 1
Case.occupied: 0 for free, 1 for white, 2 for black
Case.ind_pawn: -1 if no pawn
*/

bool pawnAlive(pawn p);
int NON(int b);
Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int ind_eat);
void destroyRafle(Rafle *rafle);
void delete_liste(Liste *liste);
int giveIndPawn(int lig, int col, pawn pawns[]);
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
void print_pawns(pawn pawns[]);
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);
int pawn_move(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche);
int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);
bool becomeDame(pawn p);