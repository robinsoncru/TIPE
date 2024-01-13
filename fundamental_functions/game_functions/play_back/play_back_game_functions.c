#include "play_back_game_functions.h"

void pawnMovedBack(Game *g, bool is_white, int ind, bool left) {
    // Redéplace juste le pion en supposant que les amis ont été redéplacés avant

    is_white = !is_white;
    // With the call of the previous function, the color changed
   
    pawn *p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;
    int di = is_white ? -1 : 1;
    int dj = left ? 1 : -1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
}

Game *record_game(Game *g) {
    Game *record_game = malloc(sizeof(Game));
    *record_game = *g;
    return record_game;
}

void annuler_coup(Game **g, Game *previous_g) {
    // Warning: mettre l'adresse du pointeur g pour modifier par effet de bord
    // free_game(*g);
    *g=previous_g;
}

// VIRER TOUT CA ET FAIRE ENREGISTRER TOUTES LES MODIFS DANS UNE LISTE CHAINE AVEC APPEL FONCTION ET 
// DANS GAME G

// void moveBack(Game *g, Kmaillon *l); // ?

// void promotionBack(Game *g, Kmaillon *l);

// void lienAmitieBack(int lig, int col, Game *g, Kmaillon *l);

// void lienEnnemitieBack(int lig, int col, Game *g, Kmaillon *l);

// void biDeplBack(Game *g, int ind, bool color, Kmaillon *l);

// void queenDeplBack(Game *g, int ind, bool color, queen_move_t tuple_coord, Kmaillon *l);