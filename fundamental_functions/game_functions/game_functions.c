#include "game_functions.h"
#include <SDL2/SDL_stdinc.h>
#include <stdlib.h>

// Logic functions

int NON(int b){
    return (b + 1) % 2;
}

bool freeCase(Case c)
{
    return c.ind_pawn == -1;
}

//la direction est donnee sous la forme d'un couple
//(dx, dy) ou dx et dy peuvent valoir -1 ou 1.
//On lui associe un nombre entre 0 et 3 inclus
//dont le bit de poids faible est la direction horizontale
//et le bit de poids fort la direction verticale.
//0 sens negatif, 1 sens positif
Uint8 getCodeFromDirs(int dj, int di){

    Uint8 weak = (dj == 1) ? 1 : 0;
    Uint8 strong = (di == 1) ? 1 : 0;
    return (strong << 1)|(weak);
}
int dir(int a){
    return (a == 0) ? -1 : 1;
}

void getDirsFromCode(Uint8 c, int* di, int* dj){
    *dj = dir(c % 2);
    *di = dir((c >> 1) % 2);
}

//Checks if the (i, j) position is out of bounds
bool outOfBounds(int i, int j){
    return i < 0 || i >= NB_CASE_LG || j < 0 || j >= NB_CASE_LG;
}

// Checks if a case is in the game
bool inGame(int lig, int col)
{
    return !outOfBounds(lig, col);
}

//Checks if eating from position (i, j) in the (add0, add1) direction leads to an out of bounds position
bool eatingIsOutOfBounds(int i, int j, int add0, int add1){
    return outOfBounds(i + 2 * add0, j + 2 * add1);
}

//checks if a pawn is allowed to move forward
//in the indicated direction
bool canMove(Game* g, bool is_white, int ind, bool left){
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;
    Coord finalPos = {g->allPawns[is_white][ind].lig + di,
                      g->allPawns[is_white][ind].col + dj};
    bool posInGame = inGame(finalPos.i, finalPos.j);
    bool wayIsFree = freeCase(g->damier[finalPos.i][finalPos.j]);
    return  posInGame && wayIsFree;
}

bool canBePromoted(Game* g, bool is_white, int ind){
    pawn* p = &(g->allPawns[is_white][ind]);
    int border = is_white ? (NB_CASE_LG - 1) : 0;
    bool isOnBorder = p->lig == border;
    bool ennmyIsDead = p->ennemy != NEUTRAL_IND && !g->allPawns[!is_white][p->ennemy].alive;
    return !p->queen && (isOnBorder || ennmyIsDead);
}

int abs(int a){
    return (a >= 0) ? a : -a;
}

//On suppose que les coordonnees sont bien sur la meme diagonale.
//retourne :
//-- 0 si la diagonale est vide
//-- 1 s'il y a exactement un pion adverse et aucun pion allie
//-- 2 s'il y a au moins deux pions peu importe leurs camps, ou au moins un pion allie.
//Si au moins un pion a ete croise, la valeur stockee a l'adresse pointee par pos
//se verra attribuer les coordonnes du premier en partant de la position initiale
Uint8 diagCount(Game* g, bool is_white, Coord initPos, Coord finalPos, Coord* pos){
    int di = (finalPos.i > initPos.i) ? 1 : -1;
    int dj = (finalPos.j > initPos.j) ? 1 : -1;
    Coord dir = {di, dj};
    int dist = abs(finalPos.i - initPos.i);

    Coord testedPos;
    int pawnCount = 0;
    int ind;
    Case* c;
    for (int k = 1; k < dist; k++) {
        testedPos = add(initPos, mult(k, dir));
        *c = g->damier[testedPos.i][testedPos.j];
        if(!freeCase(*c)){
            if (pawnCount == 1 || c->pawn_color == is_white) {
                return 2;
            }
            else {
                pawnCount = 1;
                *pos = testedPos;
            }
        }
    }
    return pawnCount;
}

//returns wether the indicated move is a diagonal
//finishing in the board
bool queenAuxCheck(Coord initPos, Coord finalPos){
    return inGame(finalPos.i, finalPos.j) && abs(finalPos.i - initPos.i) == abs(finalPos.j - initPos.j);
}

//on suppose que le pion indique est une dame.
//indique si la dame peut faire le deplacement en entree.
//retourne :
//-- 0 si elle peut se deplacer sans manger de pion
//-- 1 si elle peut se deplacer en mangeant un pion
//-- 2 si elle ne peut pas se deplacer
Uint8 queenCanMoveOrEat(Game* g, bool is_white, int ind, Coord finalPos, Coord* pos){
    Coord initPos = {
        g->allPawns[is_white][ind].lig,
        g->allPawns[is_white][ind].col
    };
    //il est preferable de ne faire qu'un seul appel a diagCount,
    //quitte a rendre le code un peu moins compact
    if (queenAuxCheck(initPos, finalPos) && freeCase(g->damier[finalPos.i][finalPos.j])) {
        return diagCount(g, is_white, initPos, finalPos, pos);
    }
    return 2;
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
    pawns[ind].ffriend = -1;
    pawns[ind].ennemy = -1;

}

// We are sure about the pawn we delete (no check control so be careful)
//Kills the indicated pawn and applies all the necessary rules
void killPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int j)
{
    if (!freeCase(damier[i][j]))
    {
        int indPawn = damier[i][j].ind_pawn;
        if (pawns[indPawn].ennemy != -1)
        {
            Npawns[pawns[indPawn].ennemy].ennemy = -1;
            Npawns[pawns[indPawn].ennemy].queen = true;
        }
        if (pawns[indPawn].ffriend != -1){
            Npawns[pawns[indPawn].ffriend].ffriend = -1;
        }
        pawn_default_value(pawns, indPawn, pawns[indPawn].color);
        damier[i][j].ind_pawn = -1;
    }
}

/* Put the pawn in a specific case (lig, col). 
Useful for queen_move and can be used by us 
to go faster for checks : place pawns where we want */
void change_pawn_place(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int lig, int col)
{
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

// For eatPawn and rafle calculation
bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    return (freeCase(damier[i + 2 * add0][j + 2 * add1]) && damier[i + add0][j + add1].pawn_color == !pawns[ind].color &&
            !freeCase(damier[i + add0][j + add1]));
}

//Version de changeForEat qui n'imprime pas les changements effectues
//Entree : deux tableaux de pions, un damier, l'index du pion qui mange, les coordonnees i et j dudit pion
//et deux entiers add0 et add1 qui indiquent la direction dans laquelle manger
//Sortie : modifie le plateau de maniere a ce que le pion d'indice ind ait mange dans la direction indique
//et retourne l'index du pion mange
int nonLoggingChangeForEat(pawn pawns[], pawn NPawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int indEater, int i, int j, int add0, int add1){

    int indVictim = damier[i + add0][j + add1].ind_pawn;
    //assert(ind > -1);
    damier[i][j].ind_pawn = VOID_INDEX;
    damier[i + 2 * add0][j + 2 * add1].pawn_color = pawns[indEater].color;
    damier[i + 2 * add0][j + 2 * add1].ind_pawn = indEater;
    //printf("pawn which is eaten %d\n", damier[i + add0][j + add1].ind_pawn);

    NPawns[indVictim].alive = false;
    damier[i + add0][j + add1].ind_pawn = VOID_INDEX;
    pawns[indEater].lig = i + 2 * add0;
    pawns[indEater].col = j + 2 * add1;
    //printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return indVictim;
}

// Play functions
//Elles ont toutes des effets de bord
//et on suppose que les coups joues sont legaux

//moves the indicated pawn frontward depending on the
//indicated direction.
//On suppose que ce coup est legal.
//Met fin au tour pour la structure de jeu
void pawnMove(Game* g, bool is_white, int ind, bool left){
    pawn* p = &(g->allPawns[is_white][ind]);
    int i = p->lig;
    int j = p->col;
    int di = is_white ? 1 : -1;
    int dj = left ? -1 : 1;

    change_pawn_place(g->allPawns[is_white], g->damier, ind, i + di, j + dj);
    if (p->ffriend != NEUTRAL_IND) {
        g->ind_move_back = p->ffriend;
        //implementer une fonction qui se charge de faire reculer
        //le pion indique a partir de son indice
    }
    endTurnGameManagement(g, is_white, ind);
}

// Queen functions
//on suppose que les coups joues sont legaux

//promotes the indicated pawn
void promote(Game* g, bool is_white, int ind){
    pawn* p = &(g->allPawns[is_white][ind]);
    p->queen = true;
    if (p->ennemy != NEUTRAL_IND) {
        pawn* ennemyPawn = &(g->allPawns[!is_white][p->ennemy]);
        killPawn(g->allPawns[is_white], g->allPawns[!is_white], g->damier, p->lig, p->col);
    }
}

void queenMoveOrEat(Game* g, bool is_white, int ind, Coord finalPos){
    pawn* queen = &(g->allPawns[is_white][ind]);
    Coord eatenPos;
    int possibleAction = queenCanMoveOrEat(g, is_white, ind, finalPos, &eatenPos);

    change_pawn_place(g->allPawns[is_white], g->damier, ind, finalPos.i, finalPos.j);
    if (possibleAction == 1) {
        killPawn(g->allPawns[is_white], g->allPawns[!is_white], g->damier, eatenPos.i, eatenPos.j);
    }

    endTurnGameManagement(g, is_white, ind);
}


void endTurnGameManagement(Game* g, bool is_white, int indMovedPawn){
    //A penser : pour l'implementation des rafles, il faudrait changer la manniere de manger
    //au lieu de manger chaque pion au fur et a mesure qu'on deplace le mangeur
    //il vaut mieux placer le pion mangeur directement a sa position finale
    //et referencer ses victimes dans une file qu'on va defiler d'un coup
    //l'idee c'est que l'application de potentielles autres regles ne va pas interferer
    //avec l'application des rafles
    if (canBePromoted(g, is_white, indMovedPawn)) {
        promote(g, is_white, indMovedPawn);
    }
    g->is_white = !g->is_white;
    g->ind_move = NEUTRAL_IND;
    g->indCheck = IND_CHANGE_ALLOWED;
}

// Debug functions
void error()
{
    printf("check");
    fflush(stdout);
}

void print_pawn(pawn p, int ind) {
    printf("Ind %d Ennemy %d, Friend %d Color %d Alive %d\n", ind, p.ennemy, p.ffriend, p.color, p.alive);
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

void assertAndLog(bool condition, char* message){
    if (!condition) {
        printf("\n");
        printf("Erreur : %s", message);
        printf("\n");
        assert(false);
    }
}