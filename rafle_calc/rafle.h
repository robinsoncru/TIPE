#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
// Operators for Rafle structure

#ifndef COORD_STRUCT
#define COORD_STRUCT
typedef struct Coord{
    int i, j;
} coord;
#endif

typedef struct Rafle Rafle;
Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int i, int j);
int lengthRafle(Rafle* rafle);
coord peekRafle(Rafle* rafle);
coord popRafle(Rafle* rafle);
void printRafle(Rafle* rafle);
void destroyRafle(Rafle *rafle);