#include "little_linked_list.h"

void push(maillon *l, int k)
{
    /* le maillon l est le maillon de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau maillon */
    maillon *m = malloc(sizeof(maillon));
    m->ind = k;
    m->next = l->next;
    l->next = m;
}

bool is_empty(maillon *l)
{
    return (l->next == NULL);
}

maillon *create_list()
{
    /* Creer une liste vide */
    maillon *l = malloc(sizeof(maillon));
    l->ind = -1;
    l->next = NULL;
    return l;
}

int pop(maillon *l)
{
    /* Supprime et renvoie le maillon en première position */
    assert(!is_empty(l));
    maillon *m = l->next;
    l->next = m->next;
    int k = m->ind;
    free(m);
    return k;
}

// For the other Kchain_list

void pushK(Kmaillon *l, int indPawnMoved, int indPawnMovedBack, int indPawnFriendlyMoved,
           int indPawnEnnemyMoved, int indQueenPromoted, int indQueenMoved)
{
    Kmaillon *m = malloc(sizeof(Kmaillon));
    m->indices.indPawnMoved = indPawnMoved;
    m->indices.indPawnMovedBack = indPawnMovedBack;
    m->indices.indPawnEnnemyMoved = indPawnFriendlyMoved;
    m->indices.indPawnEnnemyMoved = indPawnMoved;
    m->indices.indQueenPromoted = indQueenPromoted;
    m->indices.indQueenMoved = indQueenMoved;
    m->next = l->next;
    l->next = m;
}

bool is_emptyK(Kmaillon *l) {
     return (l->next == NULL);
}

k_uplet popK(Kmaillon *l) {
    assert(!is_emptyK(l));
    Kmaillon *m = l->next;
    l->next = m->next;
    k_uplet k = m->indices;
    free(m);
    return k;
}

Kmaillon *create_listK() {
    Kmaillon *l = malloc(sizeof(Kmaillon));
    l->indices.indPawnMoved = -1;
    l->indices.indPawnMovedBack = -1;
    l->indices.indPawnEnnemyMoved = -1;
    l->indices.indPawnEnnemyMoved = -1;
    l->indices.indQueenPromoted = -1;
    l->indices.indQueenMoved = -1;
    l->next = NULL;
    return l;
}