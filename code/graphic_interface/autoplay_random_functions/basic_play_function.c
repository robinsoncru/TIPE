#include "basic_play_function.h"

int random_index(Game *g) {
    // Return a random index to play a random pawn. Index between 0 and g->nb_pawns[color]-1
    int ind = rand() % g->nb_pawns[g->is_white];
    printf("%d, %d - ", g->is_white, ind);
    flush();
    return ind;
}

int plannifier_index(Game *g, int nb_coups, int* l_coups) {
    int add;
    if (g->is_white) {add = 0;} else {add=1;}
    int ind = l_coups[2*nb_coups+add];
    printf("%d, %d - ", g->is_white, ind);
    flush();
    return ind;
}

void auto_put_index(Game *g, int indPawnPlayed) {
    g->ind_move = indPawnPlayed;
}

int random_play() {
    // Return a random play decision: 6 play choices
    return rand() % 7;
}

void play_a_move(int move, int ind_pawn, Game *g, GraphicCache *cache) {
    // Play a move supposed valid

    auto_put_index(g, ind_pawn);

    switch (move)
    {
    case PAWNMOVELEFT:
        onLeftUp(g, cache);
        // Attention bug
        break;

    case PAWNMOVERIGHT:
        onRightUp(g, cache);
        // Attention bug
        break;
    
    case PROMOTION:
        onPUP(g, cache);
        break;

    case BIDEPL:
        onBUP(g, cache);
        break;

    case LIENAMITIE:
        break;

    case LIENDENNEMITIE:
        break;

    case EATRAFLE:
        onUpUp(g, cache);
        break;

    default:
        assertAndLog(true, "No play choice");
        break;
    } 

    // // Petit compteur pour gerer la vitesse d'execution
    // int timer = 0;
    // while (timer < 100*MAX_TICKS) {
    //     printf(" ");
    //     timer++;
    // }
}