#include "moveTab_quick_sort.h"
#include "../esperance_calc/esperance_calc.h"
#include <stdio.h>

void exchange(MoveTab* t, int i, int j){
    Move tmp = t->tab[i];
    t->tab[i] = t->tab[j];
    t->tab[j] = tmp;
}

int compare(Game* g, AI ai, Move a, Move b){
    //returns :
    //-1 if score(a) > score(b)
    //0 if score(a) == score(b)
    //1 if score(a) < score(b)
    double scoreA = esperanceHeuristique(ai, g, a);
    double scoreB = esperanceHeuristique(ai, g, b);
    if (scoreA > scoreB) {
        return -1;
    }
    if (scoreA < scoreB) {
        return 1;
    }
    return 0;
}

void triDrapeauNeerlandais(Game* g, MoveTab* t, AI ai, int pivot, int deb, int fin,
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
    Move valPivot = t->tab[pivot];

    while (j < k) {
        switch (compare(g, ai, valPivot, t->tab[j])) {
        case -1:
            exchange(t, i, j);
            i++;
            j++;
            break;
        
        case 1:
            exchange(t, k - 1, j);
            k--;
            break;

        default: //compare(pivot, t[j]) = 0
            j++;
            break;
        }
    }

    *iReturn = i;
    *kReturn = k;
}

void quickSortAux(Game* g, MoveTab* t, AI ai, int deb, int fin){
    //trie reccurssivement la partie de tableau :
    //      deb   fin
    //       V     V
    //[ ??? | ... | ??? ]
    if (deb < fin - 1) {
        int pivot = (rand() % (fin - deb)) + deb;
        int i, k;
        triDrapeauNeerlandais(g, t, ai, pivot, deb, fin, &i, &k);
        //on a :
        //deb        i       k        fin
        // V         V       V         V
        //[ < pivot | pivot | > pivot ]
        quickSortAux(g, t, ai, deb, i);
        quickSortAux(g, t, ai, k, fin);
    }
}

void shuffle(MoveTab* t){
    int n = t->size;
    for (int i = 0; i < n; i++) {
        exchange(t, i, rand() % (i + 1));
    }
}

void moveTabQuickSort(Game* g, MoveTab* t, AI ai){
    shuffle(t);
    quickSortAux(g, t, ai, 0, t->size);
}