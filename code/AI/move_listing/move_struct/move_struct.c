#include "move_struct.h"
#include <stdio.h>
#include <stdlib.h>

char *typeNames[8] = {"pawn move", "promotion", "pawn move back", "bi deplacement",
                      "deplacement de dame", "rafle", "lien amitie", "lien ennemitie"};

void moveFreeBackwardMoveTab(Move coup)
{
    switch (coup.type) {
        case pawnMoveBackType:
            free(coup.backwardPawnMoves->tab);
            free(coup.backwardPawnMoves);
            break;

        default:
            break;
    }
}

void moveFree(Move coup){
    switch (coup.type) {
        case pawnMoveBackType:
            free(coup.backwardPawnMoves->tab);
            free(coup.backwardPawnMoves);
            break;

        case rafleType:
            pathFree(coup.rafle);
            break;
        
        default:
            break;
    }
}

void moveTabFree(MoveTab *moveTab, int startIndex, int endIndex)
{
    PathTree* lastEncounteredTree = emptyTree;
    PathTree* currentTree;
    Move currentMove;
    for (int i = startIndex; i < endIndex; i++)
    {
        currentMove = moveTab->tab[i];
        switch (currentMove.type) {
            case pawnMoveBackType:
                free(currentMove.backwardPawnMoves->tab);
                free(currentMove.backwardPawnMoves);
                break;

            case rafleType:
                pathFree(currentMove.rafle);
                break;

            default:
                break;
        }
    }
    free(moveTab->tab);
    free(moveTab);
}

void moveTabFreeTrees(MoveTab* moveTab, int startIndex, int endIndex){
    // la liberation des arbres de rafles est non triviale
    // car plusieurs coups peuvent avoir un seul et meme arbre associe
    // cependant, en remarquant que les rafles rattachees a un meme arbre
    // sont adjacentes dans le moveTab, on en deduit qu'il suffit de garder
    // en memoire le dernier arbre croise pour effacer sans erreurs
    PathTree* lastEncounteredTree = emptyTree;
    PathTree* currentTree;
    Move currentMove;
    for (int i = startIndex; i < endIndex; i++) {
        currentMove = moveTab->tab[i];
        switch (currentMove.type) {
            case rafleType:
                currentTree = currentMove.rafleTree;
                if (currentTree != lastEncounteredTree) {
                    lastEncounteredTree = currentTree;
                    pathTreeFree(currentTree);
                }
                break;
            
            default:
                break;
        }
    }
}