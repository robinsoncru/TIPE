#include "move_struct.h"
#include <stdio.h>
#include <stdlib.h>

char *typeNames[8] = {"pawn move", "promotion", "pawn move back", "bi deplacement",
                      "deplacement de dame", "rafle", "lien amitie", "lien ennemitie"};

void moveFree(Move coup)
{
    PathTree *lastEncounteredTree = emptyTree;
    if (coup.type == rafleType)
    {
        pathFree(coup.rafle);
        if (lastEncounteredTree != coup.rafleTree)
        {
            lastEncounteredTree = coup.rafleTree;
            PathTree *m = lastEncounteredTree;
            pathTreeFree(m);
        }
        coup.rafleTree = emptyTree;
    }
    if (coup.type == pawnMoveBackType)
    {
        free(coup.backwardPawnMoves->tab);
        free(coup.backwardPawnMoves);
    }
}

// la liberation des arbres de rafles est non triviale
// car plusieurs coups peuvent avoir un seul et meme arbre associe
// cependant, en remarquant que les rafles rattachees a un meme arbre
// sont adjacentes dans le moveTab, on en deduit qu'il suffit de garder
// en memoire le dernier arbre croise pour effacer sans erreurs
void moveTabFree(MoveTab *moveTab)
{
    int n = moveTab->size;
    for (int i = 0; i < n; i++)
    {
        moveFree(moveTab->tab[i]);
    }
    free(moveTab->tab);
    free(moveTab);
}