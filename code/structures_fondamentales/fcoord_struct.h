#ifndef FCOORD
#define FCOORD

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


#include <SDL2/SDL_stdinc.h>

typedef struct Coord{
    int i;
    int j;
    /*
    i est l'indice de ligne
    j l'indice de colonne
    Dessin du repere :
    |
    |
    |
    A
    |
    o-->-------
    l'origine (0, 0) est la case inferieure gauche du plateau.*/
} Coord;


typedef struct {
    Coord pos_dame;
    Coord pos_eaten_pawn;
} queen_move_t;

typedef struct {
    int ind;
    int pba;
} ind_pba_t;

typedef struct {
    int ind;
    bool b;
} ind_bool_t;

typedef struct {
    int *friendsId;
    int foe;
    bool queen;
} primary_data_t;

#endif