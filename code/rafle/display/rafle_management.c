#include "rafle_management.h"
#include <SDL2/SDL_render.h>

void displayCurrentRafle(Game* g, SDL_Renderer* render, PathTree* cacheTree, bool display_tree){
    //displays the current rafle if allowed to
    if (display_tree) {
        pathTreeDisplay(render, g -> damier, cacheTree);
    }
}

void loadCurrentRafle(Game* g, PathTree** cacheTree){
    //Loads the current rafle in the indicated adress
    printf("\n");
    if (*cacheTree != emptyTree) {
        printf("cache tree already loaded, please free the cache first");
    }
    else {
        printf("loading current rafle tree...");
        *cacheTree = rafleTreeCalc(g, g->is_white, g -> ind_move);
        printf("\nrafle tree loaded");
    }
    printf("\n");
}

void freeCurrentRafle(PathTree** cacheTree){
    //frees the current rafle in the indicated adress
    //and replaces the pointer by emptyTree
    printf("\n");
    if (*cacheTree == emptyTree) {
        printf("cache tree is already free");
    }
    else {
        printf("freeing rafle tree...");
        PathTree* m = *cacheTree;
        *cacheTree = emptyTree;
        pathTreeFree(m);
        printf("\nrafle tree freed");
    }
    printf("\n");
}

void manageCurrentRafleOnZUp(Game* g, PathTree** cacheTree, bool* display_tree){
    if (g -> ind_move == NEUTRAL_IND) {
        printf("\nNo pawn selected\n");
    }
    else {
        if (!*display_tree) {
            loadCurrentRafle(g, cacheTree);
        }
        else {
            freeCurrentRafle(cacheTree);
        }
        *display_tree = !*display_tree;
    }
}