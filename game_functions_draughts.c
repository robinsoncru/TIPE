#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
// Game pmetre
#define NB_CASE_LG 8
#define NB_CASE NB_CASE_LG *NB_CASE_LG
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
    int occupied;
    SDL_Rect rect;
    bool color;
} Case;

/*
A quoi sert cette structure, Gasse ?
typedef struct Liste Liste;
struct Liste
{
    Rafle *first;
};*/

/*Alive n'est pas un booleen ?
le pion est soit mort soit vivant non ?
il me semble que tu avais dit que "alive" contenait aussi
une information sur le camp du pion, mais c'est juste confusionnant
de confondre ces donnees*/
typedef struct
{
    int lig, col, alive;
    bool queen;
} pawn;

// Logic functions

bool pawnAlive(pawn p)
{
    // Abstract
    return (p.alive != 0);
}

int NON(int b)
{
    return (b + 1) % 2;
}

// void printRafle(Rafle *rafle) {
//     while (!isEmpty(rafle)) {
//         rafle_a_sup = rafle;
//         rafle = rafle->pt;
//         free(rafle_a_sup);
//     }
// }

/*
D'accord, je pense voir ce que tu as essayé de faire, mais il y a des moyens beaucoup plus simples
de libérer des piles.
void delete_liste(Liste *liste)
{
    // Libère proprement tous les maillons
    for (Rafle *Maillon_present = liste->first; Maillon_present != NULL; Maillon_present = Maillon_present->pt)
    {
        free(Maillon_present);
    }
}*/

/*Si je comprends bien, chaque pion a un identifiant sous forme d'un entier naturel,
et cette fonction retourne l'identifiant du pion a une position donnee s'il y en a bien un qui soit vivant
à la case indiquee, sinon, la fonction retourne -1*/
int giveIndPawn(int lig, int col, pawn pawns[])
{
    for (int i = 0; i < NB_PAWNS; i++)
    {
        if (pawns[i].lig == lig && pawns[i].col == col && pawnAlive(pawns[i]))
        {
            return i;
        }
    }
    return -1;
}

// Aux functions

bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    /*Autant la premiere condition de la conjontion consiste clairement a tester sir la deuxieme case en diagonale est ou non libre
    Autant je ne comprends absolument pas la deuxieme clause, elle teste si le pion directement en diagonale est bien un ennemi vivant ?*/
    return (damier[i + 2 * add0][j + 2 * add1].occupied == 0 && damier[i + add0][j + add1].occupied == NON(pawns[ind].alive - 1) + 1);
}

int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    // For eatPawn
    assert(ind > -1);
    damier[i + add0][j + add1].occupied = 0;
    damier[i][j].occupied = 0;
    damier[i + 2 * add0][j + 2 * add1].occupied = pawns[ind].alive;
    printf("pawn which is eaten %d\n", giveIndPawn(i + add0, j + add1, Npawns));

    Npawns[giveIndPawn(i + add0, j + add1, Npawns)].alive = 0;
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
    if (ind > -1 && pawnAlive(pawns[ind]))
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);
        if (pawns[ind].alive == 1 && i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && damier[i + 1][j - 1].occupied == 0)
            {
                damier[i + 1][j - 1].occupied = 1;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i + 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && damier[i + 1][j + 1].occupied == 0)
            {
                damier[i + 1][j + 1].occupied = 1;
                damier[i][j].occupied = 0;
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
            if (gauche && j > 0 && damier[i - 1][j - 1].occupied == 0)
            {
                damier[i - 1][j - 1].occupied = 2;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i - 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && damier[i - 1][j + 1].occupied == 0)
            {
                damier[i - 1][j + 1].occupied = 2;
                damier[i][j].occupied = 0;
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
        return IND_CHANGE_ALLOWED;
    }
    printf("pawn alive %d or ind = %d", pawnAlive(pawns[ind]), ind);
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