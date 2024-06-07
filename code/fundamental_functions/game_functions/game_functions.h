#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

#include "Source/source_header.h"
#include "aux_game_functions/aux_functions.h"
#include "management/game_management.h"
#include "../../rafle/rafle.h"
#include "play_back/play_back_game_functions.h"

// play functions

void cancelMoveBack(Game *g, int ind, bool left);
void moveBackNGE(Game *g, bool autoplay, bool use_heuristique, double (*f)(Game *), memory_move_t *mem);
void cancelAllMoveBack(Game *g, memory_move_t *mem);

void lazzyMoveBack(Game *g, int indMovePawnBack, bool left);
void cancelLazzyMoveBack(Game *g, int indMovePawnBack, bool left);

void pawnMove(Game *g, bool is_white, int ind, bool left);

/* Implementation des règles propres aux dames quantiques:
-Promotion : selectionne le pion et tape p (sur le clavier)
-Lien d'amitié (peut on avoir pls liens d'amitié pour un pion ? Nous on ne prendra que deux pions amis au max) : selectionne
(clic gauche) le pion puis clic gauche sur l'autre pion ami du camp opposé
-Ennemie pour la vie (de même) : selectionne (clic gauche) le pion puis clic droit sur l'autre pion ennemi du camp opposé
-Un pion est soit ami, soit ennemi
*/

/* PROMOTION */
void promotion(Game *g);
/* Transforme au hasard un pion en pion ennemi ou en dame ou ne fait rien */

/* LIEN AMITIE */

/* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
void lienAmitie(int lig, int col, Game *g);

// MOVE BACK FUNCTION
void moveBack(Game *g, bool autoplay, bool use_heuristique, double (*f)(Game *));
/* Machine: joue à la suite les meilleurs coups en arrière */

/* LIEN ENNEMITIE */
void lienEnnemitie(int lig, int col, Game *g);
/* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */

/* BIDEPL */
void biDepl(Game *g, int ind, bool color);

/* QUEEN DEPL */

void queenDepl(Game *g, int ind, bool color, Coord pos_dame, bool isNGE);
void queenDeplDueMoveStruct(Game *g, int ind, bool color, Coord pos_dame);

#endif // GAME_FUNCTIONS