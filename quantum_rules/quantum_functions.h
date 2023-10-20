#include <time.h>
#include "../fundamental_functions/game_functions_draughts.h"

/* Implementation des règles propres aux dames quantiques:
-Promotion
-Lien d'amitié (peut on avoir pls liens d'amitié pour un pion ? Nous on ne prendra que deux pions amis au max)
-Ennemie pour la vie (de même ?)
*/

void promotion(Game *g);
void lienAmitie(int col, int lig, Game *g);