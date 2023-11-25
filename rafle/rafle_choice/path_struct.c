#include "path_struct.h"
#include "four_pack.h"
#include <stdio.h>
#include <stdlib.h>

//some auxilliary functions
int practicalMod(int a, int m){
    return (a >= 0) ? a % m : m - 1;
}

Coord getCoordFromCode(Uint8 c){
    int di, dj;
    getDirsFromCode(c, &di, &dj);
    Coord res = {di, dj};
    return res;
}

//Actual interface
Path* pathCreate(unsigned int c){
    Path* res = malloc(sizeof(Path));
    res->c = c;
    res->n = 0;
    res->deb = 0;
    res->fin = 0;
    res->tab = malloc((c/4 + 1) * sizeof(FourPack));
    return res;
}

void pathFree(Path* path){
    free(path->tab);
    free(path);
}

int pathLength(Path* path){
    return path->n;
}

Uint8 pathGet(int k, Path* path){
    FourPack toGet = path->tab[k / 4];
    return fourPackGet(k % 4, toGet);
}

void pathSet(Uint8 c, int k, Path* path){
    FourPack* toSet = &(path->tab[k / 4]);
    fourPackSet(c, k % 4 , toSet);
}

void pathAdd(Coord x, Path* path){
    Uint8 c = getCodeFromDirs(x.j, x.i);
    pathSet(c, path->fin, path);
    path->fin = (path->fin + 1) % path->c;
    path->n ++;
}

Coord pathPeekFirstIn(Path* path){
    Uint8 c = pathGet(path->deb, path);
    return getCoordFromCode(c);
}

Coord pathPeekLastIn(Path* path){
    Uint8 c = pathGet(practicalMod(path->fin - 1, path->c), path);
    return getCoordFromCode(c);
}
Coord pathPopFirstIn(Path* path){
    Coord res = pathPeekFirstIn(path);
    path->deb = (path->deb + 1) % path->c;
    path->n--;
    return res;
}

Coord pathPopLastIn(Path* path){
    Coord res = pathPeekLastIn(path);
    path->fin = practicalMod(path->fin - 1, path->c);
    path->n--;
    return res;
}

void pathPrint(Path* path){
    printf("[");
    if (pathLength(path) > 0) {
        Uint8 c = pathGet(path->deb, path);
        coordPrint(getCoordFromCode(c));

        for (int i = path->deb + 1; i != path->fin; i = practicalMod(i + 1, path->c)) {
            c = pathGet(i, path);
            printf(", ");
            coordPrint(getCoordFromCode(c));
        }
    }
    printf("]\n");
}

void pathTabPrint(Path* path){
    printf("[");
    if (pathLength(path) > 0) {
        printf("%d", pathGet(path->deb, path));

        for (int i = path->deb + 1; i != path->fin; i = practicalMod(i + 1, path->c)) {
            printf(", %d", pathGet(i, path));
        }
    }
    printf("]\n");
}