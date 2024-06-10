#include "moveTab_quick_sort.h"
#include "../esperance_calc/esperance_calc.h"
#include <stdio.h>

void moveTabExchange(MoveTab* t, int i, int j){
    Move tmp = t->tab[i];
    t->tab[i] = t->tab[j];
    t->tab[j] = tmp;
}

void moveTabQuickSortExchange(MoveTab* t, double* values, int i, int j){
    moveTabExchange(t, i, j);
    double tmpVal = values[i];
    values[i] = values[j];
    values[j] = tmpVal;
}

int compare(Game* g, double* values, int a, int b){
    //returns :
    //-1 if score(a) > score(b)
    //0 if score(a) == score(b)
    //1 if score(a) < score(b)
    // print_state_game(g);
    double scoreA = values[a];
    // print_state_game(g);
    double scoreB = values[b];
    // print_state_game(g);
    if (scoreA > scoreB) {
        return -1;
    }
    if (scoreA < scoreB) {
        return 1;
    }
    return 0;
}

void triDrapeauNeerlandais(Game* g, MoveTab* t, double* values, int pivot, int deb, int fin,
                            int* iReturn, int* kReturn){
    //invariant de boucle :
    //deb        i       j     k        fin
    // V         V       V     V         V
    //[ < pivot | pivot | ??? | > pivot ]

    //compare(pivot, .) =
    //deb   i   j     k  fin
    // V    V   V     V   V
    //[ -1 | 0 | ??? | 1 ]

    int i = deb, j = deb, k = fin;

    while (j < k) {
        switch (compare(g, values, pivot, j)) {
        case -1:
            moveTabQuickSortExchange(t, values, i, j);
            i++;
            j++;
            break;
        
        case 1:
            moveTabQuickSortExchange(t, values, k - 1, j);
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

void quickSortAux(Game* g, MoveTab* t, double* values, int deb, int fin){
    //trie reccurssivement la partie de tableau :
    //      deb   fin
    //       V     V
    //[ ??? | ... | ??? ]
    if (deb < fin - 1) {
        int pivot = (rand() % (fin - deb)) + deb;
        int i, k;
        triDrapeauNeerlandais(g, t, values, pivot, deb, fin, &i, &k);
        //on a :
        //deb        i       k        fin
        // V         V       V         V
        //[ < pivot | pivot | > pivot ]
        quickSortAux(g, t, values, deb, i);
        quickSortAux(g, t, values, k, fin);
    }
}

void shuffle(MoveTab* t){
    int n = t->size;
    for (int i = 0; i < n; i++) {
        moveTabExchange(t, i, rand() % (i + 1));
    }
}

void moveTabQuickSort(Game* g, MoveTab* t, AI ai){
    shuffle(t);
    double* values = malloc(t->size * sizeof(double));
    for (int i = 0; i < t->size; i++) {
        values[i] = esperanceHeuristique(ai, g, t->tab[i]);
    }
    quickSortAux(g, t, values, 0, t->size);
}