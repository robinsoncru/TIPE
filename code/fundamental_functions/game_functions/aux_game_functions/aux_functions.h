#ifndef AUX_FUNCTIONS
#define AUX_FUNCTIONS

#include "../Logic_and_calculus/logic_functions.h"




void copy_remove_pawn_from_index_to_index(Game *g, int indStart, int indArrive, bool color);
// void pawn_default_value(pawn *pawns, int ind, bool color);
void pawn_default_value_new(Game *g, int ind, bool color);
/* kill the pawn and put its values as default values */
void killPawn(Game *g, Case **damier, int i, int j);
void killPawnByInd(Game *g, bool color, int ind);
/* Check if the pawn can move back to the case localised by the coord g->coordForMoveBack */
void change_pawn_place(Game *g, int ind, bool color, int lig, int col);
//  entree : un tableau de pions pawns, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
//  des entiers add0 et add1 qui indiquent la direction dans laquelle manger
//  sortie : booleen indiquant si le pion dans la direction indique peut etre mange
int nonLoggingChangeForEat(Game *g, bool color, int indEater, int i, int j, int add0, int add1);
void promote(Game *g, bool is_white, int ind);
void createPawn(Game *g, bool color, int i, int j);
void simplyPawnMove(Game *g, bool is_white, int ind, bool left);
/* Give the coord of the case where moves back a pawn when it is linked with a friend and the friend has just moved. Return (-1, -1)
    if no case available. The coord are given at g->coordForMoveBack */
void putPawnMoveBack(Game *g, bool left);

void stormBreaks(Game *g, bool color, int indSurvivor);
void AleatStormBreaks(Game *g, bool color);
void handleCloudDuePawnMoveNGE(Game *g, int indMovedPawn, ind_pba_t *survivor, cloud_chain *l);
void AleatStormBreaksNGE(Game *g, bool color, cloud_chain *load, ind_pba_t *survivor);
queen_move_t CanMoveOrEatQueen(Game *g, bool color, int lig, int col, Case **damier, int ind);

Coord give_coord(Game *g, bool iw, int ind);
// Memory functions
void free_game(Game *g);

void doubleTabInit(int t[2]);
void incrBothTab(int t[2]);
void decrBothTab(int t[2]);

#endif // AUX_FUNCTIONS