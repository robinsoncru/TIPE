#include "input_command.h"

/*







    AUX FUNCTIONS






*/

// protocol applied at the end of every turn
void endTurnGraphics(Game *g, GraphicCache *cache)
{
    // the turn changing will be donne at the end of the
    // change timer from a graphic point of view
    alert(cache, g->indCheck, CHANGE_TICKS);
    // printf("%s turn\n", g->is_white ? "white" : "black");
}

/*







   CHECK FUNCTIONS






*/

// moves the pawn after checking the move is valid
void checkPawnMove(Game *g, GraphicCache *cache, bool left)
{
    int ind = g->ind_move; // selected pawn
    bool is_white = g->is_white;

    bool validIndex = ind > -1;
    bool pawnAlive = g->allPawns[is_white][ind].alive; // ha, ha, ha, ha, pawn alive, pawn alive !
    bool isQueen = g->allPawns[is_white][ind].queen;
    bool legalMove = canMove(g, g->is_white, g->ind_move, left);
    if (!validIndex || !pawnAlive || isQueen || !legalMove)
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
    else
    {
        pawnMove(g, g->is_white, ind, left);
        endTurnGraphics(g, cache);
    }
}

void checkLienAmitie(int i, int j, Game *g, GraphicCache *cache)
{
    int iw = g->is_white;
    int ind = g->ind_move;
    i = iw ? NB_CASE_LG - i - 1 : i;
    if (isPawnValid(g) && canBeFriend(g, ind, iw, g->damier[i][j]))
    {
        lienAmitie(i, j, g);
        endTurnGraphics(g, cache);
    }
    else
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkPawnMoveBack(Game *g, GraphicCache *cache)
{
    int iw = g->is_white;
    int indBack = g->ind_move_back;
    if (indBack > -1 && g->allPawns[iw][indBack].alive)
    {
        moveBack(g);
    }
    else
        alert(cache, IND_PB, ERROR_TICKS);
}

void checkLienEnnemitie(int i, int j, Game *g, GraphicCache *cache)
{
    int iw = g->is_white;
    int ind = g->ind_move;
    i = iw ? NB_CASE_LG - i - 1 : i;
    // j = !iw ? j : NB_CASE_LG - j - 1;
    // printf("%d %d %d", i, lig, freeCase(g->damier[i][j]));
    // fflush(stdout);
    if (isPawnValid(g) && canBeEnnemy(g, ind, iw, g->damier[i][j]))
    {
        lienEnnemitie(i, j, g);
        endTurnGraphics(g, cache);
    }
    else
    {
        alert(cache, IND_PB, ERROR_TICKS);
    }
}

void checkPromotion(Game *g, GraphicCache *cache)
{
    if (canPromotion(g))
    {
        promotion(g);
        endTurnGraphics(g, cache);
    }
    else
        alert(cache, IND_PB, ERROR_TICKS);
}

void checkBiDepl(Game *g, GraphicCache *cache)
{
    if (canBiDepl(g, g->ind_move, g->is_white))
    {
        biDepl(g, g->ind_move, g->is_white);
        endTurnGraphics(g, cache);
    }
    else
        alert(cache, IND_PB, ERROR_TICKS);
}

void checkQueenDepl(Game *g, GraphicCache *cache, bool iw, int lig, int col)
{
    queen_move_t tuple_coord = CanMoveOrEatQueen(g, iw, lig, col, g->damier, g->ind_move);
    int dame_lig = tuple_coord.pos_dame.i;
    int dame_col = tuple_coord.pos_dame.j;
    if (isPawnValid(g) && dame_lig != VOID_INDEX && dame_col != VOID_INDEX)
    {
        queenDepl(g, g->ind_move, iw, tuple_coord);
        endTurnGraphics(g, cache);
    }
    else
        alert(cache, IND_PB, ERROR_TICKS);
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
            checkQueenDepl(g, cache, iw, lig, col);
        }
        else
        {
            // Else it is an ennemy link
            checkLienEnnemitie(lig, col, g, cache);
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
        checkLienAmitie(event.button.y / LG_CASE, event.button.x / LG_CASE, g, cache);
    else
        printf("No pawn selected by right click");
}

// Moves the pawns to the left.
// After verifying it's possible
void onLeftUp(Game *g, GraphicCache *cache)
{
    /* Move pawn to left, first back if possible, then forward */
    if (needPutMoveBack(g))
    {
        putPawnMoveBack(g, true);
    }
    else
    {
        
    }

    /* Si on doit d'abord deplacer vers l'arriere un de nos pion, on met a jour les coordonnees de
     deplacement arriere puis on verifie si elles son possibles. Si c'est le cas, on deplace en arriere
     sinon on deplace un pion en avant (la disjonction de cas est correct) */
    if (moveBackAvailable(g))
        checkPawnMoveBack(g, cache);
    // Don't need to update the cache, just the game
    else
        checkPawnMove(g, cache, true);
}

void onRightUp(Game *g, GraphicCache *cache)
{
    /* Move pawn to right, same */
    if (needPutMoveBack(g))
    {
        putPawnMoveBack(g, false);
    }
    else
    {
    }

    if (moveBackAvailable(g))
        checkPawnMoveBack(g, cache);
    // same
    else
        checkPawnMove(g, cache, false);
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
        endTurnGraphics(g, cache);
    }
}

void onPUP(Game *g, GraphicCache *cache)
{
    /* Promote the selected pawn */
    checkPromotion(g, cache);
}

void onJUP(Game *g, GraphicCache *cache)
{
    print_pawns(g);
}

void onHUP(Game *g, GraphicCache *cache)
{
    print_damier(g->damier, g);
}

void onBUP(Game *g, GraphicCache *cache)
{
    checkBiDepl(g, cache);
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
    // picture_this(g);
    // // Teste for the queen

    // bool iw = g->is_white;
    // int ind = g->ind_move;
    // SDL_Event event = cache->event;

    // int x = event.button.x;
    // int y = event.button.y;
    // int lig = y / LG_CASE;
    // int col =  x/ LG_CASE;

    // queen_move_t coords = CanMoveOrEatQueen(g, iw, lig, col, g->damier, ind);
    // int dame_lig = coords.pos_dame.i;
    // int dame_col = coords.pos_dame.j;
    // if (isPawnValid(g) && dame_lig != VOID_INDEX && dame_col != VOID_INDEX)
    // {
    //     queenDeplAI(g, ind, coords);
    // }
    // else
    //     printv("big queen pb");
    // // lienEnnemitieAI(g, ind, lig, col);
    // picture_this(g);

    picture_game(g, g->is_white);
    printf("max moves %d", maxMoves(g));
    MoveTab *moveTab = listMoves(g);
    moveTabFree(moveTab);
    flush();
}