#include <stdbool.h>
#include <stdlib.h>

// Operators for Rafle structure
typedef struct Coord{
    int x, y;
} coord;

typedef struct Maillon
{
    coord point;
    struct Maillon* next;
} maillon;

typedef struct Rafle {
    /*Une rafle est une simple pile de coordonnees
    correspondant à des points, l'origine du repere est en
    la case inférieure gauche du plateau, et les cases sonr reperees ainsi :
    y---------------p
                    |
                    |
                    |
                    x*/
    int taille;
    maillon* pile;

} Rafle;

Rafle *createRafle(int x, int y)
{
    Rafle* res = malloc(sizeof(Rafle));
    res -> taille = 0;
    res -> pile = NULL;   
}

bool isEmpty(Rafle *rafle)
{
    return rafle -> pile == NULL;
}

void addWithoutIncrRafle(Rafle* rafle, int x, int y);

void addRafle(Rafle *rafle, int x, int y)
{
    addWithoutIncrRafle(rafle, x, y);
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

void destroyRafle(Rafle *rafle)
{
    while (!isEmpty(rafle)) {
        popRafle(rafle);
    }
    free(rafle);
}

/*On a implémenté une structure de rafle qui est en fait une simple pile de cases,
mais j'ai (Victor D) besoin de fonctions supplémentaires pour mon algo*/

void changeLengthOfRafle(Rafle* rafle, int new_length){
    rafle -> taille = new_length;
}

void addWithoutIncrRafle(Rafle* rafle, int x, int y){
    maillon* m = malloc(sizeof(maillon));
    coord point;
    point.x = x;
    point.y = y;
    m -> point = point;
    m -> next = rafle -> pile;
    rafle -> pile = m;
}