#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

#include "management/game_management.h"





// play functions

void pawnMove(Game *g, bool is_white, int ind, bool left);

// MOVE BACK FUNCTION

void moveBack(Game *g);





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
void moveBack(Game *g);
/* Move back the pawn referred by ind_move_back to the case localised by the coord coordForMoveBack */

/* LIEN ENNEMITIE */
void lienEnnemitie(int lig, int col, Game *g);
/* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */

/* BIDEPL */
void biDepl(Game *g, int ind, bool color);
#endif // GAME_FUNCTIONS