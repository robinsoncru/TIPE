#include "score_gameboard.h"

// Le jeu est à somme nulle, l'un gagne => l'autre perd
// Il n'y a pas d'égalité possible : à définir



bool haveMajority(Game *g, bool color, float proportion) {
    return (proportion*(float)g->nb_pawns[color] >= (float)g->nb_pawns[!color])
}

bool absoluteVictory(Game *g, bool color) {
    return g->nb_pawns[!color] == 0;
}

int easyCheckScore(Game *g, float proportion) {
    bool iw = g->is_white;

    if (absoluteVictory(g, iw)) return ABSOLUTE_VICTORY;
    else if (absoluteVictory(g, !iw)) return ABSOLUTE_DEFEATE;
    else if (haveMajority(g, iw, proportion)) return VICTORY_MAJORITY;
    else if (haveMajority(g, !iw, proportion)) return DEFEATE_MAJORITY;
    else return NO_SCORE_GIVEN;
}

int lazyHeuristic(Game *g) {

}