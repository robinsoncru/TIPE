#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef GAME_FUNCTIONS_DRAUGHTS
#define GAME_FUNCTIONS_DRAUGHTS

#include "../coord/coord.h"

// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE (NB_CASE_LG * NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)

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

// #define MY_GUARD 1
// En evaluant dans les files incluant ce file, on évite la double inclusion

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
    Case damier[NB_CASE_LG][NB_CASE_LG];
    pawn allPawns[2][2 * NB_PAWNS];
    // In the impossible but theorical case where for instance whites promote all their pawns and unfortunately they all
    // become blacks, bad choice :(
    int nb_pawns[2];
    /* Access with is_white
    Keep in memory an overapproximation of the nb of pawns, but like for merge to union_find, don't decrease the value.
    Indeed, we need a free indice to create a new ennemy pawn, not necessary the first free indice */
    int ind_move, ind_move_back, indCheck;
    bool is_white;
    Coord coordForMoveBack;
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
// Calculus fonctions
bool int_to_bool(int integer);
int bool_to_int(bool b);
// Logic functions
bool freeCase(Case c);
int NON(int b);
bool basicChecks(Game *g);
/* Couronne un pion en dame et s'il avait un ennemi, celui ci meurt */
bool inGame(int lig, int col);
/* kill the pawn and put its values as default values */
void killPawn(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j);
// void pawn_default_value(pawn pawns[], int ind, bool color);
void pawn_default_value_new(Game *g, int ind, bool color);
void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col);
void change_pawn_place_new(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool color, int lig, int col);
int dir(int a);
bool outOfBounds(int i, int j);
bool eatingIsOutOfBounds(int i, int j, int add0, int add1);
int getCodeFromDirs(int horizontalDir, int verticalDir);
void getDirsFromCode(int c, int *di, int *dj);
bool canMove(Game *g, bool is_white, int ind, bool left);
bool canBePromoted(Game *g, bool is_white, int ind);

// Aux functions
// 1:alive | 2:ennemy | 3:friendly | 4:queen | 5:lig | 6:col | 7:pba | 8:color
//  entree : un tableau de pions pawns, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
//  des entiers add0 et add1 qui indiquent la direction dans laquelle manger
//  sortie : booleen indiquant si le pion dans la direction indique peut etre mange
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int changeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
int nonLoggingChangeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1);
void put_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify, int value);
/* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */

int get_pawn_value(Game *g, bool color, int ind, int wich_pmetre_modify);
/* 1 : alive |2 : ennemy |3 : friendly |4 : queen |5 : lig |6 : col |7 : pba */

// play functions
void endTurnGameManagement(Game *g, bool is_white, int indMovedPawn, int indCheck, bool doMoveBack);
void pawnMove(Game *g, bool is_white, int ind, bool left);
void eatPawn(Game *g);
void queenDepl(int col, int lig, Game *g);
void createPawn(Game *g, bool color, int i, int j);

// Debug functions
// print functions
/* print check in the terminal and flush it */
void error();
// Affiche les caracteristiques des Pions
void print_pawns(Game *g);
// Affiche ligne, colonne et coordonnees des cases
void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG], Game *g);
// Fait planter le programme en affichant un message d'erreur
// si la condition n'est pas respectee
void assertAndLog(bool condition, char *message);
void flush();
void print_pawn(pawn p, int ind);
#endif // GAME_FUNCTIONS_DRAUGHTS