#include "little_linked_list.h"

void push(maillon *l, int k) {
    /* le maillon l est le maillon de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau maillon */
    maillon *m = malloc(sizeof(maillon));
    m->ind = k;
    m->next = l->next;
    l->next = m;
}

bool is_empty(maillon *l) {
    return (l->next == NULL);
}

maillon* create_list() {
    /* Creer une liste vide */
    maillon *l = malloc(sizeof(maillon));
    l->ind = -1;
    l->next = NULL;
    return l;
}

int pop(maillon *l) {
    /* Supprime et renvoie le maillon en première position */
    assert(!is_empty(l));
    maillon *m = l->next;
    l->next = m->next;
    int k = m->ind;
    free(m);
    return k;
}