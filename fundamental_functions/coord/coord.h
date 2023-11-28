#ifndef COORD
#define COORD

#include <stdio.h>
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

int dir(int a);
Uint8 getCodeFromDirs(int horizontalDir, int verticalDir);
void getDirsFromCode(Uint8 c, int* di, int* dj);
Coord sub(Coord ptA, Coord ptB);
Coord reduceNormOfOne(Coord v);
Coord add(Coord u, Coord v);
Coord mult(int k, Coord u);
void coordPrint(Coord x);
#endif