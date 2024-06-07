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

int random_ligne()
{
    return rand() % NB_CASE_LG;
}

Coord random_case_damier()
{
    Coord coord = {.i = random_ligne(), .j = random_ligne()};
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

Move extract_random_move_from_tab_move(MoveTab *t)
{
    assert(t->size > 0);
    int i = rand() % t->size;
    return t->tab[i];
}

int extract_pmetre_from_move(Move m)
{
    switch (m.type)
    {
    case pawnMoveType:
        if (m.left)
        {
            return PAWNMOVELEFT;
        }
        else
        {
            return PAWNMOVERIGHT;
        }
        break;

    case promotionType:
        return PROMOTION;

    case pawnMoveBackType:
        return PAWNMOVEBACK;

    case biDeplType:
        return BIDEPL;

    case queenDeplType:
        return QUEENDEPL;

    case rafleType:
        return rafleType;

    case lienAmitieType:
        return LIENAMITIE;

    case lienEnnemitieType:
        return LIENDENNEMITIE;

    default:
        assertAndLog(false, "aucun coup précalculé reconnu");
        return -1;
    }
}

int play_a_move(int move, int ind_pawn, Game *g, GraphicCache *cache, int nb_coups, int *l_coups, int *l_depl, bool isNGE)
{
    // Play a move supposed valid
    // assertAndLog(move == VOID_INDEX, "move decide dans play_a_move");

    usleep(1000 * 100);

    // MoveTab *t_moves = listMoves(g);
    // print_moves(t_moves);
    // Move m = extract_random_move_from_tab_move(t_moves);
    // move = extract_pmetre_from_move(m);
    // printf("choisi :\n");
    // print_move(m);
    int indFriend;
    Coord coord, coordi;
    if (!isNGE)
    {

        auto_put_index(g, ind_pawn);
        switch (move)
        {
        case PAWNMOVELEFT:
            checkPawnMove(g, cache, true, true);

            break;

        case PAWNMOVERIGHT:
            checkPawnMove(g, cache, false, true);
            break;

        case PROMOTION:
            onPUP(g, cache, true);
            break;

        case BIDEPL:
            onBUP(g, cache, true);
            break;

        case LIENAMITIE:
            // indFriend = plannifier_index_color(nb_coups, l_depl, !g->is_white);
            // indFriend = random_index_color(g, !g->is_white);
            // coord = coord_from_ind(g, indFriend, !g->is_white);
            // checkLienAmitie(coord.i, coord.j, g, cache, false);
            // checkLienAmitie(m.lig, m.col, g, cache, false);
            break;

        case LIENDENNEMITIE:
            // // indFriend = plannifier_index_color(2 * nb_coups + 1, l_coups, !g->is_white);
            indFriend = random_index_color(g, !g->is_white);
            // printf("-Index enn %d-", get_pawn_value(g, !g->is_white, indFriend, ENNEMY));
            // flush();
            Coord coord = coord_from_ind(g, indFriend, !g->is_white);
            checkLienEnnemitie(coord.i, coord.j, g, cache, false);
            // // checkLienEnnemitie(m.lig, m.col, g, cache, false);

            break;

        case EATRAFLE:
            onUpUp(g, cache);
            break;

        case QUEENDEPL:
            coordi = queen_valide_case(g, g->ind_move, g->is_white);
            // Coord coordi;

            // coordi.i = l_depl[2 * nb_coups];
            // coordi.j = l_depl[2 * nb_coups + 1];
            // printf(" -> lig: %d, col %d", coordi.i, coordi.j);
            checkQueenDepl(g, cache, g->is_white, coordi.i, coordi.j, true);

            // checkQueenDepl(g, cache, g->is_white, m.coords.pos_dame.i, m.coords.pos_dame.j, true);
            break;

        case PAWNMOVEBACK:
            moveBack(g, true, false, zero_fun);
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
    else
    {
        // MoveTab *coups = listMoves(g);
        memory_move_t *mem;
        printv("ind select");

        Move m;
        // if (!g->is_white)
        // {
        // printf("%d score %f ", g->is_white, heuristique_miam_trivial(g));
        flush();
        MoveTab *t = listMoves(g);
        print_moves(t);
        // // Coord cma = {.i = 9, .j = 9};
        // // }
        // // else
        // // {
        // //     m.manipulatedPawn = random_index(g);
        // // }
        m = t->tab[20];
        print_move(m);
        printf("pion selec %d", m.manipulatedPawn);
        flush();
        mem = applyDeter(g, m, true); // Tester un eclatement de nuage
        // picture_this(g);
        for (int j = 0; j < mem->lenghtIssues; j++)
        {
            applyIssue(g, mem, j);
            // print_state_game(g, QUEEN);
            usleep(1000 * 100);
            applyRecipIssue(g, mem, j);
            // print_state_game(g, PBA);
        }
        applyRecipDeter(g, mem);
        // print_state_game(g, QUEEN);
        // print_liensAmitie(g);
        
        // endTurnGameManagement(g, g->is_white, m.manipulatedPawn, IND_CHANGE_ALLOWED, false); // Parce que ce sont des NGE
    }

    return nb_coups + 1;
}