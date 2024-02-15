#include "move_max.h"

// Soit t'es ghost

int possibleMoveForOneGhostPawn()
{
    int deplPossibles = 2;  /* Déplacement classique du pion : front left, front right, pas de retour
     arrière possible car c'est un ghost pawn */
    int biDeplPossible = 1; /* le pion peut encore se séparer en deux et avancer */
    return deplPossibles + biDeplPossible;
}

// Soit t'es pas ghost

// Donc soit t'es queen sans ami

int possibleMoveForOneQueenWithoutFriend(Game *g)
{
    // Une queen n'a pas d'ennemi, mais peut avoir un (petit) ami (genre son cousin, on parle de nobles hein)
    bool iw = g->is_white;
    int deplPossibles = NB_CASE_LG * 2;
    /* Pire cas ou la dame est au centre du damier, comprend le
    déplacement arrière forcé par un pion ami */
    int eatPossible = quickPow(4, g->nb_pawns[!iw] - g->lengthCloud[!iw]);
    /* Mange tous les pions restants avec 4 directions possibles */
    int friendPossible = g->nb_pawns[!iw] - g->nbFoe[!iw] - g->nbFriendNoQueen[!iw] - g->lengthCloud[!iw] - g->nbQueenWithFriend[!iw];
    /* La dame peut devenir ami, donc on compte les pions adverses qui peuvent encore devenir amis
    avec la dame */
    return deplPossibles + eatPossible + friendPossible;
}

// soit t'es queen avec ami

int possibleMoveForOneQueenWithFriend(Game *g)
{
    bool iw = g->is_white;

    // Une queen n'a pas d'ennemi, mais peut avoir un (petit) ami (genre le frère du roi)
    int deplPossibles = NB_CASE_LG * 2;                                    /* Pire cas ou la dame est au centre du damier, comprend le
                                         déplacement arrière forcé par un pion ami */
    int eatPossible = quickPow(4, g->nb_pawns[!iw] - g->lengthCloud[!iw]); /* Mange tous les pions restants avec 4 directions
    possibles */
    return deplPossibles + eatPossible;
}

// Soit t'es pas queen

// Donc soit t'as un ennemie

int possibleMoveForOneFoe(Game *g)
{
    bool iw = g->is_white;
    int deplPossibles = 2;                                                 /* Déplacement classique du pion : front left, front right, pas de retour
                                                    arrière possible car c'est un ennemy pawn */
    int eatPossible = quickPow(4, g->nb_pawns[!iw] - g->lengthCloud[!iw]); /* Mange tous les pions restants avec 4 directions
    possibles */
    int promotePossible = 1;                                               // Il peut être promu
    return deplPossibles + eatPossible + promotePossible;
}

// Soit t'as pas d'ennemie

// Donc soit t'as un ami

int possibleMoveForOneFriendNoQueen(Game *g)
{
    bool iw = g->is_white;
    int deplPossibles = 4;                                                 /* Déplacement classique du pion : front left, front right + retour arrière */
    int eatPossible = quickPow(4, g->nb_pawns[!iw] - g->lengthCloud[!iw]); /* Mange tous les pions restants avec 4 directions
    possibles */
    int promotePossible = 1;                                               // Il peut etre promu
    return deplPossibles + eatPossible + promotePossible;
}

// Soit t'as pas d'amis, et dans ce cas là t'es un pion normal

int possibleMoveForOneCasualPawn(Game *g)
{
    bool iw = g->is_white;
    int deplPossibles = 2;                                                 /* Déplacement classique du pion : front left, front right */
    int eatPossible = quickPow(4, g->nb_pawns[!iw] - g->lengthCloud[!iw]); /* Mange tous les pions restants avec 4 directions
    possibles */
    int promotePossible = 1;                                               // Il peut etre promu
    int biDeplPossible = 1;                                                // Il peut devenir un ghost pawn et se séparer en deux
    int friendPossible = g->nb_pawns[!iw] - g->nbFoe[!iw] - g->nbFriendNoQueen[!iw] - g->lengthCloud[!iw] - g->nbQueenWithFriend[!iw];
    int foePossible = friendPossible - g->nbQueenWithoutFriend[!iw]; /* Les pions candidats pour devenir amis sont les
     mêmes que pour devenir ennemis, moins les dames */
    return deplPossibles + eatPossible + promotePossible + biDeplPossible + friendPossible + foePossible;
}

int maxMoves(Game *g)
{
    bool iw = g->is_white;
    int casualPawns = g->nb_pawns[iw] - g->nbFoe[iw] - g->nbFriendNoQueen[iw] - g->lengthCloud[iw] - g->nbQueenWithFriend[iw] - g->nbQueenWithoutFriend[iw];
    return g->lengthCloud[iw] * possibleMoveForOneGhostPawn() + g->nbQueenWithFriend[iw] * possibleMoveForOneQueenWithFriend(g) +
           g->nbQueenWithoutFriend[iw] * possibleMoveForOneQueenWithoutFriend(g) + g->nbFoe[iw] * possibleMoveForOneFoe(g) +
           g->nbFriendNoQueen[iw] * possibleMoveForOneFriendNoQueen(g) + casualPawns * possibleMoveForOneCasualPawn(g);
}
