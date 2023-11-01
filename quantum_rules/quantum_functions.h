#include <time.h>
#include "../fundamental_functions/game_functions_draughts.h"

/* Implementation des règles propres aux dames quantiques:
-Promotion : selectionne le pion et tape p (sur le clavier)
-Lien d'amitié (peut on avoir pls liens d'amitié pour un pion ? Nous on ne prendra que deux pions amis au max) : selectionne
(clic gauche) le pion puis clic gauche sur l'autre pion ami du camp opposé
-Ennemie pour la vie (de même) : selectionne (clic gauche) le pion puis clic droit sur l'autre pion ennemi du camp opposé
*/

void promotion(Game *g);
/* Transforme au hasard un pion en pion ennemi ou en dame ou ne fait rien */
void lienAmitie(int col, int lig, Game *g);
/* Lie d'amitie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on lie d'amitié un pion qui était déjà ami, le dernier lien
    écrase le précédent */
void moveBack(Game *g, bool gauche);
/* Move back a pawn when it is linked with a friend and the friend has just moved */

void lienEnnemitie(int col, int lig, Game *g);
/* Declare ennemis pour la vie le pion en indice avec le pion se trouvant en coord (lig, col) sur le damier, en verifiant qu'il est bien de
    couleur opposé et qu'il existe. Gère le pmetre Game.friend. Si on declare ennemi un pion qui était déjà ennemi, le dernier lien
    écrase le précédent */
