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

// Game structure

typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
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
    int lig, col;
    bool alive, color, queen;
} pawn;

typedef struct
{
    pawn allPawns[2][2 * NB_PAWNS];
    // In the impossible but theorical case where for instance whites promote all their pawns and unfortunately they all
    // become blacks, bad choice :(
    int nb_pawns[2];
    /* Access with is_white
    Keep in memory an overapproximation of the nb of pawns, but like for merge to union_find, don't decrease the value.
    Indeed, we need a free indice to create a new ennemy pawn, not necessary the first free indice */
} Game;

/* NOTES :

pawn.alive:
pawn.color: true if white, false if black
Whites start at lig 0
Blacks start at lig NB_CASE_LG - 1
Case.pawn_color: true for white, false for black
Case.ind_pawn: -1 if no pawn
*/

// Basic functions
bool freeCase(Case c);
int NON(int b);
bool becomeDame(pawn p);
bool inGame(int lig, int col);
void popPawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j);
void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col);

Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int ind_eat);
void destroyRafle(Rafle *rafle);
// Ci dessus: Je crois que je vais les supprimer lors du prochain merge (Victor G)

bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
// entree : un tableau de pions pawns, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
// des entiers add0 et add1 qui indiquent la direction dans laquelle manger
// sortie : booleen indiquant si le pion dans la direction indique peut etre mange
int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int pawn_move(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche);
int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);
int queenDepl(int col, int lig, bool is_white, pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);

// For tests
void print_pawns(pawn pawns[]);
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);