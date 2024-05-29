#ifndef PLAY_BACK_GAME_FUNCTIONS
#define PLAY_BACK_GAME_FUNCTIONS


#include "../management/game_management.h"
#include "../../../rafle/calc/rafle_tree_calc.h"
#include "../../../rafle/rafle_eating/rafle_eating.h"


// cancel a move, turn the game for the other player and cancel the changes for the move (queen promotion ...): liste chaine qui contient les transfo durant les moves


void pawnMoveNGE(Game *g, bool is_white, int ind, bool left);

void pawnMoveCancel(Game *g, bool is_white, int ind, bool left);

void recreateCloud(Game *g, cloud_chain *l, int indFromCaseSurvivor, int pbaSurvivor, bool iw);
ind_bool_t biDeplNGE(Game *g, bool color, int ind);
void cancelBidepl(Game *g, int oldInd, ind_bool_t data);
int promotionNGE(Game *g, int ind);
void cancelPromotion(Game *g, int indPawnBeforeProm, Coord pos_potential_foe_from_prom);

void cancelDeplQueen(Game *g, int ind_queen, data_chain *chainy, Coord init_coord);

data_chain *rafleNGE(Game *g, int indMovePawn);
void cancelRafle(Game *g, int indMovedPawn, Coord init_pos, data_chain *chainy);

void lienAmitieNGE(int lig, int col, Game *g, int indPawn);
void cancelLienAmitie(Game *g, int indPawn, int lig, int col);
void lienAmitiePmetreNGE(int lig, int col, int ind, bool is_white, Game *g);

void lienEnnemitieNGE(int lig, int col, Game *g, int indPawn);
void cancelLienEnnemitie(Game *g, int indPawn, int lig, int col);
void lienEnnemitiePmetreNGE(bool is_white, int lig, int col, int ind, Game *g);
data_chain *queenDeplNGE(Game *g, int ind, bool color, Coord pos_dame);
#endif