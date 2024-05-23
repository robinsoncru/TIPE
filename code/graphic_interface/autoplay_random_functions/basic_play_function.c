#include "basic_play_function.h"

int random_index_color(Game *g, bool color)
{
    assertAndLog(enoughPawns(g, color), "Tous les pions sont morts");
    // Return a random index to play a random pawn. Index between 0 and g->nb_pawns[color]-1
    int ind = rand() % g->nb_pawns[color];
    printf("%d, %d - ", color, ind);
    flush();
    return ind;
}

int random_index(Game *g)
{
    return random_index_color(g, g->is_white);
}

int random_ligne() {
    return rand() % NB_CASE_LG;
}

Coord random_case_damier() {
    Coord coord = {.i=random_ligne(), .j=random_ligne()};
    return coord;
}

int plannifier_index_color(int nb_coups, int *l_coups, bool color)
{
    int ind = l_coups[nb_coups];
    printf("%d, %d - ", color, ind);
    flush();
    return ind;
}

int plannifier_index(Game *g, int nb_coups, int *l_coups)
{
    return plannifier_index_color(nb_coups, l_coups, g->is_white);
}

void auto_put_index(Game *g, int indPawnPlayed)
{
    g->ind_move = indPawnPlayed;
}

int random_play()
{
    // Return a random play decision: 6 play choices
    return rand() % 7;
}

int play_a_move(int move, int ind_pawn, Game *g, GraphicCache *cache, int nb_coups, int *l_coups, int *l_depl)
{
    // Play a move supposed valid

    usleep(1000*500);

    auto_put_index(g, ind_pawn);

    switch (move)
    {
    case PAWNMOVELEFT:
        onLeftUp(g, cache, true);
        // Attention bug
        break;

    case PAWNMOVERIGHT:
        onRightUp(g, cache, true);
        // Attention bug
        break;

    case PROMOTION:
        onPUP(g, cache, true);
        break;

    case BIDEPL:
        onBUP(g, cache, true);
        break;

    case LIENAMITIE:
        // int indFriend = plannifier_index_color(g, nb_coups, l_coups, !g->is_white);
        // int indFriend = random_index_color(g, !g->is_white);
        // // printf("-Index friend %d-", get_pawn_value(g, !g->is_white, indFriend, FRIENDLY));
        // // flush();
        // Coord coord = coord_from_ind(g, indFriend, !g->is_white);
        // checkLienAmitie(coord.i, coord.j, g, cache, false);
        break;

    case LIENDENNEMITIE:
        int indFriend = plannifier_index_color(2*nb_coups+1, l_coups, !g->is_white);
        // int indFriend = random_index_color(g, !g->is_white);
        printf("-Index enn %d-", get_pawn_value(g, !g->is_white, indFriend, ENNEMY));
        flush();
        Coord coord = coord_from_ind(g, indFriend, !g->is_white);
        checkLienEnnemitie(coord.i, coord.j, g, cache, false);
        break;

    case EATRAFLE:
        onUpUp(g, cache);
        break;

    case QUEENDEPL:
        Coord coordi = queen_valide_case(g, g->ind_move, g->is_white);
        // Coord coordi;
        // coordi.i = l_depl[2*nb_coups];
        // coordi.j = l_depl[2*nb_coups+1];
        printf(" -> lig: %d, col %d", coordi.i, coordi.j);
        checkQueenDepl(g, cache, g->is_white, coordi.i, coordi.j, true);
        break;

    case PAWNMOVEBACKLEFT:
        onLeftUp(g, cache, true);
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
    return nb_coups+1;
}