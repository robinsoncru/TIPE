#include <SDL2/SDL_main.h>
#include <stdio.h>
#include <assert.h>
#include "rafle_aux.h"

//Compile : gcc test_rafle.c rafle.c rafle_calc.c ../game_functions_draughts.c -o test_struct_rafle
int main(int argc, char** argv){
    printf("Création de la rafle...\n");
    Rafle* r = createRafle();
    printf("Rafle créée.\n");

    assert(isEmpty(r));

    int tabx[] = {};
    int taby[] = {};
    addWithoutIncrRafle(r, int x, int y)

    printf("Supression de la rafle...\n");
    destroyRafle(r);
    printf("Rafle supprimée.\n");
    return 0;
}