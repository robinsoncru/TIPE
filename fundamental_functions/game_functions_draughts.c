#include "game_functions_draughts.h"

// Logic functions

bool freeCase(Case c)
{
    return c.ind_pawn == -1;
}

bool outOfBounds(int i, int j) {
    //Checks if the (i, j) position is out of bounds
    return i < 0 || i >= NB_CASE_LG || j < 0 || j >= NB_CASE_LG;
}

int NON(int b)
{
    return (b + 1) % 2;
}

bool becomeDame(pawn p, pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG])
{
    if (p.alive && !p.queen)
    {
        if (p.color)
        {
            if (p.lig == NB_CASE_LG - 1)
            {
                if (p.ennemy != -1)
                    popPawn(Npawns, pawns, damier, Npawns[p.ennemy].lig, Npawns[p.ennemy].col);
                return true;
            }
            else
                return false;
        }
        else
        {
            if (p.lig == 0)
            {
                if (p.ennemy != -1)
                    popPawn(Npawns, pawns, damier, Npawns[p.ennemy].lig, Npawns[p.ennemy].col);
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

bool inGame(int lig, int col)
{
    // Check if a case is in the game
    return (-1 < lig && lig < NB_CASE_LG && -1 < col && col < NB_CASE_LG);
}

// Aux functions

void pawn_default_value(pawn pawns[], int ind, bool init_is_white)
{
    /* Initialize pawn with default values */
    pawns[ind].alive = false;
    pawns[ind].col = -1;
    pawns[ind].lig = -1;
    pawns[ind].queen = false;
    pawns[ind].color = init_is_white;
    pawns[ind].friend = -1;
    pawns[ind].ennemy = -1;

}

void popPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j)
{
    // We are sur about the pawn we delete (no check control so be careful)
    if (!freeCase(damier[i][j]))
    {
        if (pawns[damier[i][j].ind_pawn].ennemy != -1)
        {
            Npawns[pawns[damier[i][j].ind_pawn].ennemy].ennemy = -1;
            Npawns[pawns[damier[i][j].ind_pawn].ennemy].queen = true;
        }
        if (pawns[damier[i][j].ind_pawn].friend != -1)
            Npawns[pawns[damier[i][j].ind_pawn].friend].friend = -1;
        pawn_default_value(pawns, damier[i][j].ind_pawn, pawns[damier[i][j].ind_pawn].color);
        damier[i][j].ind_pawn = -1;
    }
}

void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col)
{
    /* Put the pawn in a specific case (lig, col). Useful for queen_move and can be used by us to go faster for checks : place
    pawns where we want */
    damier[pawns[ind].lig][pawns[ind].col].ind_pawn = -1;
    pawns[ind].lig = lig;
    pawns[ind].col = col;
    damier[lig][col].ind_pawn = ind;
    damier[lig][col].pawn_color = pawns[ind].color;
}

/* May be useful later */
// Case forbiddenCase()
// {
//     /* Return a case that is a case error. When a function return this case, that mean it doesn't return any case (it doesn't find
//     an allowed case...) */
//     Case c;
//     c.color = false;
//     c.ind_pawn = IND_PB; // That's the important thing
//     c.pawn_color = false;
//     return c;
// }

bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    return (freeCase(damier[i + 2 * add0][j + 2 * add1]) && damier[i + add0][j + add1].pawn_color == !pawns[ind].color &&
            !freeCase(damier[i + add0][j + add1]));
}

int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    assert(ind > -1 && pawns[ind].alive && !pawns[ind].queen);
    // printf("pawn which is eaten %d\n", damier[i + add0][j + add1].ind_pawn);

    change_pawn_place(pawns, damier, ind, i + 2 * add0, j + 2 * add1);
    popPawn(Npawns, pawns, damier, i + add0, j + add1);
    // printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);

    // Check if the pawn moved become a queen

    if (ind > -1 && becomeDame(pawns[ind], pawns, Npawns, damier))
    {
        pawns[ind].queen = true;
        // printf("Become dame %d", ind);
        // fflush(stdout);
    }

    return IND_CHANGE_ALLOWED;
}

// Play functions

int pawnMovePmetre(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche, Game *g)
{
    if (ind > -1 && pawns[ind].alive && !pawns[ind].queen)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);
        if (pawns[ind].color && i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j - 1);
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i + 1, j + 1);
            }
            else
            {
                // printf("white ind pb occ\n");
                return IND_PB;
            }
        }
        else if (!pawns[ind].color && i > 0)
        {
            if (gauche && j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j - 1);
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                change_pawn_place(pawns, damier, ind, i - 1, j + 1);
            }

            else
            {
                // printf("black idn pb occ\n");
                return IND_PB;
            }
        }
        else
        {
            // printf("color pb\n");
            return IND_PB;
        }

        // Check if the pawn moved become a queen

        if (ind > -1 && becomeDame(pawns[ind], pawns, g->allPawns[!g->is_white], damier))
        {
            pawns[ind].queen = true;
        }

        g->ind_move_back = pawns[ind].friend;

        return IND_CHANGE_ALLOWED;
    }
    // printf("pawn alive %d or ind = %d", pawns[ind].alive, ind);
    return IND_PB;
}

void pawnMove(Game *g, bool gauche)
{
    g->ind_check = pawnMovePmetre(g->allPawns[g->is_white], g->damier, g->ind_move, gauche, g);
    g->ind_move = -1;
}

int eatPawnPmetre(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    // printf("call EatPawn \nind pawn which eats %d\n", ind);
    if (ind > -1 && pawns[ind].alive && !pawns[ind].queen)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);

        /*wtf, il teste toutes les directions dans lequel le pion peut manger ? OUI */
        if (i > 1 && j > 1 && canEat(pawns, damier, ind, i, j, -1, -1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, -1, -1);
        else if (i < NB_CASE_LG - 2 && j > 1 && canEat(pawns, damier, ind, i, j, 1, -1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, 1, -1);
        else if (i > 1 && j < NB_CASE_LG - 2 && canEat(pawns, damier, ind, i, j, -1, 1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, -1, 1);
        else if (i < NB_CASE_LG - 2 && j < NB_CASE_LG - 2 && canEat(pawns, damier, ind, i, j, 1, 1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, 1, 1);
        else
        {
            // printf("no condition satisfied");
            return IND_PB;
        }
    }
    // printf("ind = -1\n");
    return NEUTRAL_IND;
}

void eatPawn(Game *g)
{
    g->ind_check = eatPawnPmetre(g->allPawns[g->is_white], g->allPawns[!g->is_white], g->damier, g->ind_move);
    g->ind_move = -1;
}
// Queen functions

bool MoveOrEatQueen(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    // Check if the move of the queen is possible and move the queen or eat the next pawn in her path
    assert(pawns[ind].alive && ind > -1);
    pawn p = pawns[ind];
    int dcol = col - p.col;
    int dlig = lig - p.lig;
    Case c;
    if (abs(dlig) == abs(dcol) && dcol != 0)
    {
        int add_lig = dlig / abs(dlig);
        int add_col = dcol / abs(dcol);
        for (int i = 1; i < abs(dcol) + 1; i++)
        {
            c = damier[p.lig + add_lig * i][p.col + add_col * i];
            if (!freeCase(c))
            {
                // Here the queen can eat a pawn
                int new_lig = add_lig + p.lig + add_lig * i;
                int new_col = add_col + p.col + add_col * i;
                // Check if the queen can go here
                if (c.pawn_color == !p.color && inGame(new_lig, new_col) && freeCase(damier[new_lig][new_col]))
                {
                    change_pawn_place(pawns, damier, ind, new_lig, new_col);
                    popPawn(Npawns, pawns, damier, new_lig - add_lig, new_col - add_col);
                    return true;
                }
                else if (c.pawn_color == p.color)
                {
                    // printf("jump sheep");
                    return false; // Pb: the queen jumps a pawn of her own color
                }
            }
        }
        // Only move the pawn: nothing in her path
        if (inGame(lig, col) && freeCase(damier[lig][col]))
        {
            change_pawn_place(pawns, damier, ind, lig, col);
            return true;
        }
    }
    return false; // No case was found
}

int queenDeplPmetre(int col, int lig, bool is_white, pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    if (is_white)
        lig = NB_CASE_LG - lig - 1;
    if (MoveOrEatQueen(pawns, Npawns, lig, col, damier, ind))
        return IND_CHANGE_ALLOWED;
    else
        return IND_PB;
}

void queenDepl(int col, int lig, Game *g)
{
    g->ind_move_back = g->allPawns[g->is_white][g->ind_move].friend;
    g->ind_check = queenDeplPmetre(col, lig, g->is_white, g->allPawns[g->is_white], g->allPawns[!g->is_white], g->damier, g->ind_move);
    g->ind_move = -1;
}

// Debug functions
void error()
{
    printf("check");
    fflush(stdout);
}

void print_pawn(pawn p, int ind) {
    printf("Ind %d Ennemy %d, Friend %d Color %d Alive %d\n", ind, p.ennemy, p.friend, p.color, p.alive);
    fflush(stdout);
}

void print_pawns(Game *g)
{
    for (int i = 0; i < 2*NB_PAWNS; i++)
    {
        print_pawn(g->allPawns[g->is_white][i], i);
    }
}

void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG])
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            printf("Case ligne %d col %d pos: %d, %d\n", i, j, damier[i][j].rect.x, damier[i][j].rect.y);
        }
    }
}