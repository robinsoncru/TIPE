#include "move_struct.h"
#include <stdio.h>
#include <stdlib.h>

char* typeNames[8] = {"pawn move", "promotion", "pawn move back", "bi deplacement",
"deplacement de dame", "rafle", "lien amitie", "lien ennemitie"};


void moveTabFree(MoveTab* moveTab){
    int n = moveTab->size;
    for (int i = 0; i < n; i++) {
        if (moveTab->tab[i].type == rafleType) {
            pathFree(moveTab->tab[i].rafle);
            pathTreeFree(moveTab->tab[i].rafleTree);
        }
    }
    free(moveTab->tab);
    free(moveTab);
}