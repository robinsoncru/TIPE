#include "game_functions_draughts.h"

// Logic functions

bool freeCase(Case c)
{
    return c.ind_pawn == VOID_INDEX;
}

int NON(int b)
{
    return (b + 1) % 2;
}

int getCodeFromDirs(int dj, int di){
    //la direction est donnee sous la forme d'un couple
    //(dx, dy) ou dx et dy peuvent valoir -1 ou 1
    //on lui associe un nombre entre 0 et 3 inclus
    //dont le bit de poids faible est la direction horizontale
    //et le bit de poids fort la direction verticale
    //0 sens negatif, 1 sens positif
    int weak = (dj == 1) ? 1 : 0;
    int strong = (di == 1) ? 1 : 0;
    return (strong << 1)|(weak);
}

void getDirsFromCode(int c, int* di, int* dj){
    *dj = dir(c % 2);
    *di = dir((c >> 1) % 2);
}

int dir(int a){
    return (a == 0) ? -1 : 1;
}

bool outOfBounds(int i, int j){
    //Checks if the (i, j) position is out of bounds
    return i < 0 || i >= NB_CASE_LG || j < 0 || j >= NB_CASE_LG;
}

bool eatingIsOutOfBounds(int i, int j, int add0, int add1){
    //Checks if eating from position (i, j) in the (add0, add1) direction leads to an out of bounds position
    return outOfBounds(i + 2 * add0, j + 2 * add1);
}

int xMinusSing(int x){
    int sg = (x >= 0) ? 1 : -1;
    return x - sg;
}

void vectorToEatenPawn(int vi, int vj, int* ei, int* ej){
    //Entree : vecteur entre deux positions apres avoir mange un pion
    //Sortie : vecteur de la position initiale au pion mange
    *ei = xMinusSing(vi);
    *ej = xMinusSing(vj);
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

int nonLoggingChangeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1){
    //Version de changeForEat qui n'imprime pas les changements effectues
    //Entree : deux tableaux de pions, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
    //et deux entiers add0 et add1 qui indiquent la direction dans laquelle manger
    //Sortie : modifie le plateau de maniere a ce que le pion d'indice ind ait mange dans la direction indique
    //et retourne l'index du pion mange

    int indVictim = damier[i + add0][j + add1].ind_pawn;
    //assert(ind > -1);
    damier[i][j].ind_pawn = VOID_INDEX;
    damier[i + 2 * add0][j + 2 * add1].pawn_color = pawns[ind].color;
    damier[i + 2 * add0][j + 2 * add1].ind_pawn = ind;
    //printf("pawn which is eaten %d\n", damier[i + add0][j + add1].ind_pawn);

    Npawns[indVictim].alive = false;
    damier[i + add0][j + add1].ind_pawn = VOID_INDEX;
    pawns[ind].lig = i + 2 * add0;
    pawns[ind].col = j + 2 * add1;
    //printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return indVictim;
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