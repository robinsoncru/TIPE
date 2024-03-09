#include "path_struct.h"
#include "four_pack.h"
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>

//some auxilliary functions
int practicalMod(int a, int m){
    return (a >= 0) ? a % m : m - 1;
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

void pathAdd(Uint8 c, Path* path){
    pathSet(c, path->fin, path);
    path->fin = (path->fin + 1) % path->c;
    path->n ++;
}

Path* pathCopy(Path* path){
    Path* res = pathCreate(path->c);
    res->n = path->n;
    res->deb = path->deb;
    res->fin = path->fin;
    for (int k = 0; k < path->c; k++) {
        pathSet(pathGet(k, path), k, res);
    }
    return res;
}

Uint8 pathPeekFirstIn(Path* path){
    Uint8 c = pathGet(path->deb, path);
    return c;
}

Uint8 pathPeekLastIn(Path* path){
    Uint8 c = pathGet(practicalMod(path->fin - 1, path->c), path);
    return c;
}
Uint8 pathPopFirstIn(Path* path){
    Uint8 res = pathPeekFirstIn(path);
    path->deb = (path->deb + 1) % path->c;
    path->n--;
    return res;
}

Uint8 pathPopLastIn(Path* path){
    Uint8 res = pathPeekLastIn(path);
    path->fin = practicalMod(path->fin - 1, path->c);
    path->n--;
    return res;
}

void pathPrint(Path* path){
    printf("[");
    if (pathLength(path) > 0) {
        printf("%d", pathGet(path->deb, path));

        for (int i = path->deb + 1; i != path->fin; i = practicalMod(i + 1, path->c)) {
            printf(", %d", pathGet(i, path));
        }
    }
    printf("]\n");
}