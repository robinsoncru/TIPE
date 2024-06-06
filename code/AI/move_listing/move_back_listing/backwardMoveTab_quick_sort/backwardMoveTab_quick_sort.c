#include "backwardMoveTab_quick_sort.h"
#include <stdio.h>

void exchange(backwardMoveTab_t* t, int i, int j){
    backwardMove_t tmp = t->tab[i];
    t->tab[i] = t->tab[j];
    t->tab[j] = tmp;
}

int compare(Game* g, backwardMove_t a, backwardMove_t b){
    //returns :
    //-1 if a > b
    //0 if a == b
    //1 if a < b
    pawn pA = get_pawn(g, g->is_white, a.indMovedPawn);
    pawn pB = get_pawn(g, g->is_white, b.indMovedPawn);

    int perspective = g->is_white ? 1 : -1;

    Coord toCompareA = {.i = perspective * pA.lig,
                        .j = pA.col};
    Coord toCompareB = {.i = perspective * pB.lig,
                        .j = pB.col};
    
    if (toCompareA.i != toCompareB.i) {
        return (toCompareA.i < toCompareB.i) ? 1 : -1;
    }
    //toCompareA.i == toCompareB.i
    if (toCompareA.j != toCompareB.j) {
        return (toCompareA.j < toCompareB.j) ? 1 : -1;
    }
    return 0;
}

void triDrapeauNeerlandais(Game* g, backwardMoveTab_t* t, int pivot, int deb, int fin,
                            int* iReturn, int* kReturn){
    //invariant de boucle :
    //deb        i       j     k        fin
    // V         V       V     V         V
    //[ < pivot | pivot | ??? | > pivot ]

    //lexCmp(pivot, .) =
    //deb   i   j     k  fin
    // V    V   V     V   V
    //[ -1 | 0 | ??? | 1 ]

    int i = deb, j = deb, k = fin;
    backwardMove_t valPivot = t->tab[pivot];

    while (j < k) {
        switch (compare(g, valPivot, t->tab[j])) {
        case -1:
            exchange(t, i, j);
            i++;
            j++;
            break;
        
        case 1:
            exchange(t, k - 1, j);
            k--;
            break;

        default:
            j++;
            break;
        }
    }

    *iReturn = i;
    *kReturn = k;
}

void quickSortAux(Game* g, backwardMoveTab_t* t, int deb, int fin){
    //trie reccurssivement la partie de tableau :
    //      deb   fin
    //       V     V
    //[ ??? | ... | ??? ]
    if (deb < fin) {
        int pivot = (rand() % (fin - deb)) + deb;
        int i, k;
        triDrapeauNeerlandais(g, t, pivot, deb, fin, &i, &k);
        //on a :
        //deb        i       k        fin
        // V         V       V         V
        //[ < pivot | pivot | > pivot ]
        quickSortAux(g, t, deb, i);
        quickSortAux(g, t, k, fin);
    }
}

void shuffle(backwardMoveTab_t* t){
    int n = t->n;
    for (int i = 0; i < n; i++) {
        exchange(t, i, rand() % (i + 1));
    }
}

void backwardMoveTabQuickSort(Game* g, backwardMoveTab_t *t){
    shuffle(t);
    quickSortAux(g, t, 0, t->n);
}