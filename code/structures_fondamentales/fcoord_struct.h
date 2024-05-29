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


// typedef struct {
//     Coord pos_dame;
//     Coord pos_eaten_pawn;
// } queen_move_t;

typedef struct {
    int ind;
    int pba;
} ind_pba_t;

typedef struct {
    int ind;
    bool b;
} ind_bool_t;

typedef struct int_chain int_chain;

struct int_chain
{
    int *tableau;
    int ind_actu;
    int size_max;
};



typedef struct {
    Coord *tab;
    int sizetab;
    int index_actu;
} coord_tab_t;

typedef struct {
    coord_tab_t *friendsId;
    Coord pos_foe;
    bool queen;
} primary_data_t;

#endif