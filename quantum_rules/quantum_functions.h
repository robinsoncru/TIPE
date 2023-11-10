#ifndef QUANTUM_RULES
#define QUANTUM_RULES

#include <time.h>
#include "../fundamental_functions/game_functions/game_functions.h"

/* Implementation des règles propres aux dames quantiques:
-Promotion : selectionne le pion et tape p (sur le clavier)
-Lien d'amitié (peut on avoir pls liens d'amitié pour un pion ? Nous on ne prendra que deux pions amis au max) : selectionne
(clic gauche) le pion puis clic gauche sur l'autre pion ami du camp opposé
-Ennemie pour la vie (de même) : selectionne (clic gauche) le pion puis clic droit sur l'autre pion ennemi du camp opposé
*/


/* PROMOTION */
void promotion(Game *g);
/* Transforme au hasard un pion en pion ennemi ou en dame ou ne fait rien */

/* LIEN AMITIE */
void lienAmitie(int col, int lig, Game *g);
/* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
void putPawnMoveBack(Game *g, bool left);
/* Give the coord of the case where moves back a pawn when it is linked with a friend and the friend has just moved. Return (-1, -1)
    if no case available. The coord are given at g->coordForMoveBack */
bool moveBackNotAvailable(Game *g);
/* Check if the pawn can move back to the case localised by the coord g->coordForMoveBack */
void moveBack(Game *g);
/* Move back the pawn referred by ind_move_back to the case localised by the coord coordForMoveBack */

/* LIEN ENNEMITIE */
void lienEnnemitie(int col, int lig, Game *g);
/* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */
#endif // QUANTUM_RULES