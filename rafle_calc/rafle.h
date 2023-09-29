#include <stdbool.h>
// Operators for Rafle structure
typedef struct Coord{
    int i, j;
} coord;
typedef struct Rafle Rafle;
Rafle *createRafle();
bool isEmpty(Rafle *rafle);
void addRafle(Rafle *rafle, int ind_eat);
int lengthRafle(Rafle* rafle);
coord peekRafle(Rafle* rafle);
coord popRafle(Rafle* rafle);
void printRafle(Rafle* rafle);
void destroyRafle(Rafle *rafle);