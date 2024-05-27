#include "move_struct.h"
#include <stdio.h>
#include <stdlib.h>

char* typeNames[8] = {"pawn move", "promotion", "pawn move back", "bi deplacement",
"deplacement de dame", "rafle", "lien amitie", "lien ennemitie"};


//la liberation des arbres de rafles est non triviale
//car plusieurs coups peuvent avoir un seul et meme arbre associe
//cependant, en remarquant que les rafles rattachees a un meme arbre
//sont adjacentes dans le moveTab, on en deduit qu'il suffit de garder
//en memoire le dernier arbre croise pour effacer sans erreurs
void moveTabFree(MoveTab* moveTab){
    PathTree* lastEncounteredTree = emptyTree;
    int n = moveTab->size;
    for (int i = 0; i < n; i++) {
        if (moveTab->tab[i].type == rafleType) {
            pathFree(moveTab->tab[i].rafle);
            if (lastEncounteredTree != moveTab->tab[i].rafleTree) {
                lastEncounteredTree = moveTab->tab[i].rafleTree;
                PathTree* m = lastEncounteredTree;
                pathTreeFree(m);
            }
            moveTab->tab[i].rafleTree = emptyTree;
        }
    }
    free(moveTab->tab);
    free(moveTab);
}