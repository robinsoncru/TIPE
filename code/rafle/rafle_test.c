#include "rafle_test.h"
#include "rafle_choice/path_struct.h"
#include <stdlib.h>

#define CAPACITY 65

int main(int argc, char* charv[]){
    srand(177013);
    Path* path = pathCreate(CAPACITY);

    for (int k = 0; k < CAPACITY; k++) {
        pathAdd((Uint8)(rand() % 256), path);
    }

    for (int k = 0; k < CAPACITY/2; k++) {
        pathPopFirstIn(path);
        pathAdd((Uint8)(rand() % 256), path);
    }

    Path* copy = pathCopy(path);
    pathPrint(path);
    pathPrint(copy);

    pathFree(copy);
    pathFree(path);
    return 0;
}