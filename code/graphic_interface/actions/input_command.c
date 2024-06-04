#include "input_command.h"
#include <stdio.h>

/*







    AUX FUNCTIONS






*/

// protocol applied at the end of every turn
void endTurnGraphics(Game *g, GraphicCache *cache)
{
    // the turn changing will be donne at the end of the
    // change timer from a graphic point of view
    alert(cache, g->indCheck, CHANGE_TICKS);
}

/*







   CHECK FUNCTIONS






*/

// moves the pawn after checking the move is valid
void checkPawnMove(Game *g, GraphicCache *cache, bool left, bool autoplay)
{
    int ind = g->ind_move; // selected pawn
    bool is_white = g->is_white;

    assertAndLog(isValidIndex(ind), "Pion depl index non valide");
    bool isQueen = int_to_bool(get_pawn_value(g, is_white, ind, QUEEN));

    bool legalMove = canMove(g, g->is_white, g->ind_move, left);
    if (!legalMove || isQueen)
    {
        if (autoplay)
        {
            assertAndLog(false, "Depl pion pas valide ou pion dame");
        }
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else
    {
        pawnMove(g, g->is_white, ind, left);
        // endTurnGraphics(g, cache);
    }
}

void checkLienAmitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch)
{
    // Lors du jeu automatique, l'écran ne tourne pas donc screen_switch vaut false
    int iw = g->is_white;
    int ind = g->ind_move;
    if (screen_switch)
    {
        i = iw ? NB_CASE_LG - i - 1 : i;
    }
    if (isPawnValid(g) && canBeFriend(g, ind, iw, get_case_damier(g, i, j)))
    {
        lienAmitie(i, j, g);
        // endTurnGraphics(g, cache);
    }
    else
    {
        if (!screen_switch)
        {
            // C'est la machine qui joue
            assertAndLog(false, "Pion peut pas être amis ou index pas valide");
        }
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkLienEnnemitie(int i, int j, Game *g, GraphicCache *cache, bool screen_switch)
{
    // Lors du jeu automatique, l'écran ne tourne pas donc screen_switch vaut false

    int iw = g->is_white;
    int ind = g->ind_move;
    if (screen_switch)
    {
        i = iw ? NB_CASE_LG - i - 1 : i;
    }
    if (isPawnValid(g) && canBeEnnemy(g, ind, iw, get_case_damier(g, i, j)))
    {
        lienEnnemitie(i, j, g);
        // endTurnGraphics(g, cache);
    }
    else
    {
        if (!screen_switch)
        {
            // C'est la machine qui joue
            assertAndLog(false, "Pion peut pas être ennemi ou index pas valide");
        }
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkPromotion(Game *g, GraphicCache *cache, bool autoplay)
{
    if (canPromotion(g))
    {
        promotion(g);
        // endTurnGraphics(g, cache);
    }
    else
    {
        if (autoplay)
        {
            // C'est la machine qui joue
            assertAndLog(false, "Pion peut pas être promu ou index pas valide");
        }
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkBiDepl(Game *g, GraphicCache *cache, bool autoplay)
{
    if (canBiDepl(g, g->ind_move, g->is_white))
    {
        biDepl(g, g->ind_move, g->is_white);
        // endTurnGraphics(g, cache);
    }
    else
    {
        if (autoplay)
        {
            // C'est la machine qui joue
            assertAndLog(false, "Pion peut pas être ghost depl ou index pas valide");
        }
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkQueenDepl(Game *g, GraphicCache *cache, bool iw, int lig, int col, bool autoplay)
{
    // Jeu plante si une machine joue un coup non valide, alors qu'on avertit
    // simplement l'utilisateur avec un message s'il fait un coup non valide et le coup est annulé
    // On distingue les deux avec autoplay
    Coord pos_dame = CanMoveOrEatQueen(g, iw, lig, col, g->ind_move, !autoplay);
    int dame_lig = pos_dame.i;
    int dame_col = pos_dame.j;
    // isPawnValid(g);
    // if (g->ind_move == 3) pawnMove(g, g->is_white, g->ind_move, true);
    if (isPawnValid(g) && dame_lig != VOID_INDEX && dame_col != VOID_INDEX && int_to_bool(get_pawn_value(g, g->is_white, g->ind_move, QUEEN)))
    {
        queenDepl(g, g->ind_move, iw, pos_dame, false);
        // endTurnGraphics(g, cache);
    }
    else
    {
        if (autoplay)
        {
            assertAndLog(false, "Reine: index pas valide ou case arrive pas valide");
        }
        else
        {
            alert(cache, IND_PB, ERROR_TICKS);
        }
    }
}

/*







    BUTTON FUNCTIONS






*/

// Click on a pawn to select it
void onLMBDown(Game *g, GraphicCache *cache)
{
    SDL_Event event = cache->event;

    int lig = event.button.y / LG_CASE;
    int col = event.button.x / LG_CASE;
    bool iw = g->is_white;

    if (g->indCheck == IND_NORMAL && g->ind_move == -1)
        // Je garde les conditions pour eviter un comportement indefini, je veux que selectPawn ne soit
        // appeler que sous des conditions bien precise
        selectPawn(g, event.button.x, event.button.y);

    else if (g->indCheck == IND_NORMAL && g->ind_move > VOID_INDEX)
    {
        // printv("ind norm but ind move no");
        if (int_to_bool(get_pawn_value(g, iw, g->ind_move, QUEEN)))
        {
            // printv("now check if queen");
            // If the pawn is a queen, then it is a queen depl, we verify if the movement is available
            checkQueenDepl(g, cache, iw, lig, col, false);
        }
        else
        {
            // Else it is an ennemy link
            checkLienEnnemitie(lig, col, g, cache, true);
        }
    }

    else if (g->ind_move == VOID_INDEX)
        printf("No pawn selected");
    else
    {
        printv("bouton gauche bad config");
        assert(false);
    }
}

void onRMBDown(Game *g, GraphicCache *cache)
{
    SDL_Event event = cache->event;
    if (g->indCheck == IND_NORMAL)
        checkLienAmitie(event.button.y / LG_CASE, event.button.x / LG_CASE, g, cache, true);
    else
        printf("No pawn selected by right click");
}

// Moves the pawns to the left.
// After verifying it's possible
void onLeftUp(Game *g, GraphicCache *cache, bool autoplay)
{
    /* Move pawn to left, first back if possible, then forward */
    assertAndLog(false, "pour inds_move_back, la fonction onleftup n'est plus valide");
    // if (needPutMoveBack(g))
    // {
    //     putPawnMoveBack(g, true);
    // }

    // /* Si on doit d'abord deplacer vers l'arriere un de nos pion, on met a jour les coordonnees de
    //  deplacement arriere puis on verifie si elles son possibles. Si c'est le cas, on deplace en arriere
    //  sinon on deplace un pion en avant (la disjonction de cas est correct) */
    // if (moveBackAvailable(g))
    //     checkPawnMoveBack(g, cache, autoplay);
    // // Don't need to update the cache, just the game
    // else
    //     checkPawnMove(g, cache, true, autoplay);
}

void onRightUp(Game *g, GraphicCache *cache, bool autoplay)
{
    /* Move pawn to right, same */
    assertAndLog(false, "pour inds_move_back, la fonction onrightup n'est plus valide");

    // if (needPutMoveBack(g))
    // {
    //     putPawnMoveBack(g, false);
    // }
    // else
    // {
    // }

    // if (moveBackAvailable(g))
    //     checkPawnMoveBack(g, cache, autoplay);
    // // same
    // else
    //     checkPawnMove(g, cache, false, autoplay);
}

void onEscapeUp(Game *g, GraphicCache *cache)
{
    cache->is_playing = false;
}

void onUpUp(Game *g, GraphicCache *cache)
{
    if (g->ind_move == NEUTRAL_IND || pathTreeDepth(g->currentTree) == 0)
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else if (g->currentTree == emptyTree)
    {
        bool isWhite = g->is_white;
        g->currentTree = rafleTreeCalc(g, isWhite, g->ind_move);
        cache->display_tree = true;
    }
    else if (!cache->display_tree)
    {
        cache->display_tree = true;
    }
    else
    {
        printf("lazyRafle called\n");
        Path *r = lazyRafle(g->currentTree);
        printf("eatRafle called\n");
        eatRafle(g, g->ind_move, g->is_white, g->currentTree, r);
        printf("pathFree called\n");
        pathFree(r);
        // endTurnGraphics(g, cache);
    }
}

void onPUP(Game *g, GraphicCache *cache, bool autoplay)
{
    /* Promote the selected pawn */
    checkPromotion(g, cache, autoplay);
}

void onJUP(Game *g, GraphicCache *cache)
{
    print_pawns(g, g->is_white);
}

void onHUP(Game *g, GraphicCache *cache)
{
    print_damier(g->damier, g);
}

void onBUP(Game *g, GraphicCache *cache, bool autoplay)
{
    checkBiDepl(g, cache, autoplay);
}

void onLUP(Game *g)
{
    print_little_linked_list(g->cloud[g->is_white]);
}

void onAUP(GraphicCache *cache)
{
    cache->autoplay = !cache->autoplay;
}

void onZUp(Game *g, GraphicCache *cache)
{
    int indEater = g->ind_move;
    if (indEater == NEUTRAL_IND)
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else if (pathTreeDepth(g->currentTree) > 0)
    {
        cache->display_tree = !cache->display_tree;
    }
    else
    { // no tree loaded ans the pawn is already selected
        bool isWhite = g->is_white;
        g->currentTree = rafleTreeCalc(g, isWhite, indEater);
        cache->display_tree = true;
    }
}

void onKUp(Game *g, GraphicCache *cache)
{
    print_damier(g->damier, g);
}

void onRUp(Game *g, GraphicCache *cache)
{

    // MoveTab *coups = listMoves(g);
    // memory_move_t *mem;
    // for (int i = 0; i < coups->size; i++)
    // {
    //     mem = applyDeter(g, coups->tab[i]);
    //     picture_this(g);
    //     for (int j = 0; j < mem->lenghtIssues; j++)
    //     {
    //         applyMove(g, mem, j);
    //         applyRecipMove(g, mem);
    //     }
    //     applyRecipDeter(g, mem);
    // }
    // else
    //     printv("big queen pb");
    // // lienEnnemitieIssue(g, ind, lig, col);
    // picture_this(g);

    int majoration, actualNbCoups;
    majoration = maxMoves(g);
    picture_game(g, g->is_white);
    printf("majoration pour la memoire : %d", majoration);
    MoveTab *moveTab = listMoves(g);
    actualNbCoups = moveTab->size;
    printf("\nnombre de coups : %d\n", moveTab->size);
    printf("utilite memoire : %f\n", ((float) actualNbCoups) / ((float) majoration));
    print_moves(moveTab);
    moveTabFree(moveTab);
    flush();
}
