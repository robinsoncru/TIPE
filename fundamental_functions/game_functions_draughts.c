#include "game_functions_draughts.h"

// Game structure

#ifndef PION_ET_PLATEAU
#define PION_ET_PLATEAU
typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
} Case;

typedef struct
{
    int lig, col;
    bool alive, color, queen;
} pawn;
#endif /*PION_ET_PLATEAU*/

// Logic functions

bool freeCase(Case c)
{
    return c.ind_pawn == VOID_INDEX;
}

int NON(int b)
{
    return (b + 1) % 2;
}

bool becomeDame(pawn p)
{
    if (p.alive && !p.queen)
    {
        if (p.color)
        {
            if (p.lig == NB_CASE_LG - 1)
                return true;
            else
                return false;
        }
        else
        {
            if (p.lig == 0)
                return true;
            else
                return false;
        }
    }
    return false;
}

// Aux functions

bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    return (freeCase(damier[i + 2 * add0][j + 2 * add1]) && damier[i + add0][j + add1].pawn_color == !pawns[ind].color &&
            !freeCase(damier[i + add0][j + add1]));
}

int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    assert(ind > -1);
    damier[i][j].ind_pawn = -1;
    damier[i + 2 * add0][j + 2 * add1].pawn_color = pawns[ind].color;
    damier[i + 2 * add0][j + 2 * add1].ind_pawn = ind;
    printf("pawn which is eaten %d\n", damier[i + add0][j + add1].ind_pawn);

    Npawns[damier[i + add0][j + add1].ind_pawn].alive = false;
    damier[i + add0][j + add1].ind_pawn = -1;
    pawns[ind].lig = i + 2 * add0;
    pawns[ind].col = j + 2 * add1;
    printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return IND_CHANGE_ALLOWED;
}

// Print functions for checks

void print_pawns(pawn pawns[])
{
    for (int i = 0; i < NB_PAWNS; i++)
    {
        printf("Sel %d\n", pawns[i].alive);
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

// Play functions

int pawn_move(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche)
{
    printf("call pawn_move\n");
    if (ind > -1 && pawns[ind].alive)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);
        if (pawns[ind].color && i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && freeCase(damier[i + 1][j - 1]))
            {
                damier[i + 1][j - 1].pawn_color = true;
                pawns[ind].lig = i + 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i + 1][j + 1]))
            {
                damier[i + 1][j + 1].pawn_color = true;
                pawns[ind].lig = i + 1;
                pawns[ind].col = j + 1;
            }
            else
            {
                printf("white ind pb occ\n");
                return IND_PB;
            }
        }
        else if (i > 0)
        {
            if (gauche && j > 0 && freeCase(damier[i - 1][j - 1]))
            {
                damier[i - 1][j - 1].pawn_color = false;
                pawns[ind].lig = i - 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && freeCase(damier[i - 1][j + 1]))
            {
                damier[i - 1][j + 1].pawn_color = false;
                pawns[ind].lig = i - 1;
                pawns[ind].col = j + 1;
            }

            // printf("ind %d\n", ind);
            // printf("%d %d\n", i, j);
            // printf("%d %d\n", pawns[ind].lig, pawns[ind].col);

            else
            {
                printf("black idn pb occ\n");
                return IND_PB;
            }
        }
        else
        {
            printf("color pb\n");
            return IND_PB;
        }
        damier[i][j].ind_pawn = -1;
        damier[pawns[ind].lig][pawns[ind].col].ind_pawn = ind;
        return IND_CHANGE_ALLOWED;
    }
    printf("pawn alive %d or ind = %d", pawns[ind].alive, ind);
    return IND_PB;
}

int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    printf("call EatPawn \nind pawn which eats %d\n", ind);
    if (ind > -1)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d move %d NcanEat %d", i, j, move_direction, canEat(pawns, damier, ind, i, j, 1, -1));
        printf("%d %d", i, j);

        /*wtf, il teste toutes les directions dans lequel le pion peut manger ?*/
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
            printf("no condition satisfied");
            return IND_PB;
        }
    }
    printf("ind = -1\n");
    return NEUTRAL_IND;
}