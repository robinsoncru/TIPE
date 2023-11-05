#ifndef COORD
#define COORD
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

Coord sub(Coord ptA, Coord ptB);
Coord add(Coord u, Coord v);
Coord mult(int k, Coord u);
#endif