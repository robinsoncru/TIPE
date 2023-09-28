#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Operators for Rafle structure
typedef struct Coord{
    int i, j;
} coord;

typedef struct Maillon
{
    coord point;
    struct Maillon* next;
} maillon;

typedef struct Rafle {
    /*Une rafle est une simple pile de coordonnees
    correspondant à des points, l'origine du repere est en
    [A completer]*/

    int taille;
    maillon* pile;

} Rafle;

Rafle *createRafle()
{
    Rafle* res = malloc(sizeof(Rafle));
    res -> taille = 0;
    res -> pile = NULL;
    return res;
}

bool isEmpty(Rafle *rafle)
{
    return rafle -> pile == NULL;
}

void addWithoutIncrRafle(Rafle* rafle, int i, int j);

void addRafle(Rafle *rafle, int i, int j)
{
    addWithoutIncrRafle(rafle, i, j);
    rafle -> taille++;
}

int lengthRafle(Rafle* rafle){
    return rafle -> taille;
}

coord peekRafle(Rafle* rafle){
    return rafle -> pile -> point;
}

coord popRafle(Rafle* rafle){
    coord res = rafle -> pile -> point;
    maillon* m = rafle -> pile;
    rafle -> pile = rafle -> pile -> next;
    free(m);

    rafle -> taille--;
    return res;
}

void printCoord(coord point){
    printf("(%d, %d)", point.i, point.j);
}

void printRafle(Rafle* rafle){
    
    if (isEmpty(rafle)) {
        printf("Rafle vide.");
    }else {
        maillon* p = rafle -> pile;
        printCoord(p -> point);
        p = p -> next;

        while (p != NULL) {
            printf(" -> ");
            printCoord(p -> point);
            p = p -> next;
        }
    }
}

void emptyRafle(Rafle* rafle);

void destroyRafle(Rafle *rafle)
{
    emptyRafle(rafle);
    free(rafle);
}

/*On a implémenté une structure de rafle qui est en fait une simple pile de cases,
mais j'ai (Victor D) besoin de fonctions supplémentaires pour mon algo*/

void changeLengthOfRafle(Rafle* rafle, int new_length){
    rafle -> taille = new_length;
}

void addWithoutIncrRafle(Rafle* rafle, int i, int j){
    maillon* m = malloc(sizeof(maillon));
    coord point;
    point.i = i;
    point.j = j;
    m -> point = point;
    m -> next = rafle -> pile;
    rafle -> pile = m;
}

void emptyRafle(Rafle* rafle){
    while (!isEmpty(rafle)) {
        popRafle(rafle);
    }
    rafle -> taille = 0;
}