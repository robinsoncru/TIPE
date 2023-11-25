#include "path_struct.h"
#include <stdlib.h>
#include <stdio.h>

#define RNG_INIT 3102023
#define TESTS_ITERS 100
#define MAX_CAP 500

int rngNegOrPosOne(){
    return (rand() % 2 == 0) ? -1 : 1;
}

int main(int argc, char* argv[]){
    srand(RNG_INIT);

    unsigned int lengthTab[TESTS_ITERS];
    for (int i = 0; i < TESTS_ITERS; i++) {
        lengthTab[i] = (rand() % MAX_CAP) + 1;
    }

    Coord x;
    Path* path;
    for (int i = 0; i < TESTS_ITERS; i++) {
        printf("----------------------\n iteration number %d\n", i + 1);
        path = pathCreate(lengthTab[i]);
        printf("path created\n");
        for (int j = 0; j < lengthTab[i]; j++) {
            x.i = rngNegOrPosOne();
            x.j = rngNegOrPosOne();
            printf("adding :");
            coordPrint(x);
            printf("\n");
            pathAdd(x, path);
            pathPrint(path);
            //pathTabPrint(path);
        }
        printf("path filled\n\n");
        for (int j = 0; j < lengthTab[i]; j++) {
            switch (j % 2) {
                case 0:
                    printf("poping first in :");
                    coordPrint(pathPopFirstIn(path));
                break;

                case 1:
                    printf("poping last in :");
                    coordPrint(pathPopLastIn(path));
                break;
            }
            printf("\n");
            pathPrint(path);
            //pathTabPrint(path);
        }
        printf("path emptied\n");
        Path* m = path;
        path = NULL;
        pathFree(m);
        printf("path freed\n");
    }
    return EXIT_SUCCESS;
}