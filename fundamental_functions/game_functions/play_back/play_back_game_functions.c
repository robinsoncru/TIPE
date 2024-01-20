#include "play_back_game_functions.h"

int pawnMoveNoGraphicEffect(Game *g, bool is_white, int ind, bool left)
{
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i + di, j + dj);
    if (p.friendly != NEUTRAL_IND)
    {
        return p.friendly;
        // le pion indique a partir de son indice
    }
    return VOID_INDEX;
}

void pawnMoveBackNoGraphicEffect(Game *g, bool is_white, int ind, bool left)
{
    pawn p = g->allPawns[is_white][ind];
    int i = p.lig;
    int j = p.col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place_new(g, g->damier, ind, is_white, i - di, j - dj);
}

int biDeplNoGraphicEffect(Game *g, bool color, int ind) {
    // On suppose le coup legal
    bool depl = int_to_bool(rand() % 2);
    // Depl le pion a droite ou a gauche et creera l'autre ghost pawn de l'autre cote
    int dj = depl ? 1 : -1;
    int di = color ? 1 : -1;
    // Creer un pion a droite ou a gauche aleatoirement
    int newLig = get_pawn_value(g, color, ind, LIG) + di;
    int newCol = get_pawn_value(g, color, ind, COL) + dj;
    createPawn(g, color, newLig, newCol);
    int newInd = g->damier[newLig][newCol].ind_pawn;
    put_pawn_value(g, color, newInd, PBA, get_pawn_value(g, color, ind, PBA) * 2);

    // Rajoute dans le cloud
    if (!isInCloud(g, color, ind))
        push(g->cloud[color], ind);
    push(g->cloud[color], newInd);

    g->lengthCloud[color]++;

    // Deplace le pion de l'autre cote
    put_pawn_value(g, color, ind, PBA, get_pawn_value(g, color, ind, PBA) * 2);
    simplyPawnMove(g, color, ind, depl);

}

// void moveBack(Game *g, Kmaillon *l); // ?

// void promotionBack(Game *g, Kmaillon *l);

// void lienAmitieBack(int lig, int col, Game *g, Kmaillon *l);

// void lienEnnemitieBack(int lig, int col, Game *g, Kmaillon *l);

// void biDeplBack(Game *g, int ind, bool color, Kmaillon *l);

// void queenDeplBack(Game *g, int ind, bool color, queen_move_t tuple_coord, Kmaillon *l);