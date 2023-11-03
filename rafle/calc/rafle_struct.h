#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../fundamental_functions/game_functions_draughts.h"
// Operators for Rafle structure

typedef struct Rafle Rafle;
Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int i, int j);
int lengthRafle(Rafle* rafle);
Coord peekRafle(Rafle* rafle);
Coord popRafle(Rafle* rafle);
void printRafle(Rafle* rafle);
void destroyRafle(Rafle *rafle);