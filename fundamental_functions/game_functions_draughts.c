#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE (NB_CASE_LG * NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)
#define IND_PB -3
#define IND_CHANGE_ALLOWED -2
#define NEUTRAL_IND -1
#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3

// Game structure

typedef struct
{
    bool pawn_color, color;
    int ind_pawn;
    SDL_Rect rect;
} Case;

typedef struct Rafle Rafle;
struct Rafle
{
    int ind_eat;
    Rafle *pt;
};

typedef struct Liste Liste;
struct Liste
{
    Rafle *first;
};

typedef struct
{
    int lig, col;
    bool alive, color, queen;
} pawn;

// Logic functions

bool freeCase(Case c)
{
    return c.ind_pawn == -1;
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

bool inGame(int lig, int col)
{
    // Check if a case is in the game
    return (-1 < lig && lig < NB_CASE_LG && -1 < col && col < NB_CASE_LG);
}

// Operators for Rafle structure

Rafle *createRafle()
{
    Rafle *rafle = malloc(sizeof(Rafle));
    rafle->pt = NULL;
    rafle->ind_eat = -1;
    return rafle;
}

bool isEmpty(Rafle *rafle)
{
    return (rafle->pt == NULL);
}

void addRafle(Rafle *rafle, int ind_eat)
{
    rafle->ind_eat = ind_eat;
    rafle->pt = createRafle();
}

void destroyRafle(Rafle *rafle)
{
    Rafle *rafle_a_sup;
    while (!isEmpty(rafle))
    {
        rafle_a_sup = rafle;
        rafle = rafle->pt;
        free(rafle_a_sup);
    }
    rafle->ind_eat = -1;
}

// void printRafle(Rafle *rafle) {
//     while (!isEmpty(rafle)) {
//         rafle_a_sup = rafle;
//         rafle = rafle->pt;
//         free(rafle_a_sup);
//     }
// }

// Aux functions

void popPawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j)
{
    // We are sur about the pawn we delete (no check control so be careful)
    if (!freeCase(damier[i][j]))
    {
        pawns[damier[i][j].ind_pawn].alive = false;
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
    popPawn(Npawns, damier, i + add0, j + add1);
    // printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);

    // Check if the pawn moved become a queen

    if (ind > -1 && becomeDame(pawns[ind]))
    {
        pawns[ind].queen = true;
        // printf("Become dame %d", ind);
        // fflush(stdout);
    }

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
        else if (i > 0)
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

        if (ind > -1 && becomeDame(pawns[ind]))
        {
            pawns[ind].queen = true;
        }

        return IND_CHANGE_ALLOWED;
    }
    // printf("pawn alive %d or ind = %d", pawns[ind].alive, ind);
    return IND_PB;
}

int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
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

// Queen functions

bool MoveOrEatQueen(pawn pawns[], pawn Npawns[], int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    // Check if the move of the queen is possible and move the queen or eat the next pawn in her path
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
                    popPawn(Npawns, damier, new_lig - add_lig, new_col - add_col);
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

int queenDepl(int col, int lig, bool is_white, pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    if (is_white)
        lig = NB_CASE_LG - lig - 1;
    if (MoveOrEatQueen(pawns, Npawns, lig, col, damier, ind))
        return IND_CHANGE_ALLOWED;
    else
        return IND_PB;
}