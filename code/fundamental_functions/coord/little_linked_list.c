#include "little_linked_list.h"

void push(int_chain *l, int k)
{
    /* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */
    assert(l->ind_actu + 1 < l->size_max);
    l->ind_actu++;
    l->tableau[l->ind_actu] = k;
}

bool is_empty(int_chain *l)
{
    assert(l->ind_actu < l->size_max);
    return (l->ind_actu <= -1);
}

int_chain *create_list(int size_max)
{
    /* Creer une liste vide */
    int_chain *m = malloc(sizeof(int_chain));
    m->ind_actu = -1;
    m->size_max = size_max;
    m->tableau = malloc(size_max * sizeof(int));
    for (int i = 0; i < size_max; i++)
    {
        m->tableau[i] = -1;
    }
    return m;
}

int pop(int_chain *l)
{
    /* Supprime et renvoie le int_chain en première position */
    assert(!is_empty(l));
    int k = l->tableau[l->ind_actu];
    l->tableau[l->ind_actu] = -1;
    l->ind_actu--;
    return k;
}

int taille_list(int_chain *l)
{
    assert(l->ind_actu < l->size_max);
    return l->ind_actu + 1;
}

int get(int_chain *l, int i)
{
    /* renvoie le int_chain en i-eme position */
    assert(!is_empty(l) && 0 <= i && i < taille_list(l));
    return l->tableau[i];
}

void seti(int_chain *l, int i, int valeur)
{
    /* push une valeur en i-eme position */
    assert(!is_empty(l) && 0 <= i && i < taille_list(l));
    l->tableau[i] = valeur;
}

void freeIntChain(int_chain *l)
{
    while (!is_empty(l))
    {
        pop(l);
    }
    free(l->tableau);
    free(l);
}

bool alreadyInList(int_chain *l, int ind)
{
    assert(l->ind_actu < l->size_max);
    for (int i = 0; i < taille_list(l); i++)
    {
        if (ind == get(l, i))
        {
            return true;
        }
    }
    return false;
}

void replaceValueInList(int_chain *l, int old_value, int new_value)
{
    // La liste est supposée sans doublon. Opération linéaire en la taille de la liste
    assert(!is_empty(l));
    for (int i = 0; i < taille_list(l); i++)
    {
        if (get(l, i) == old_value)
        {
            seti(l, i, new_value);
            break;
        }
    }
}

void popi(int_chain *l, int ind)
{
    print_int_chain(l);
    // Supprime un ind de la liste chainé et le renvoie, et diminue en conséquence sa taille
    assert(l->ind_actu < l->size_max);
    bool find = false;
    
    if (get(l, l->ind_actu) == ind)
    { // Si c'est le dernier elm

        pop(l);
        find = true;
    }
    else
    {
        for (int i = 0; i < taille_list(l) - 1; i++)
        {
            if (get(l, i) == ind)
            {
                find = true;
            }
            if (find)
            {
                seti(l, i, get(l, i + 1));
            }
        }
        pop(l);
    }
    if (!find)
    {
        assertAndLog(false, "popi :  Elm a supprime dans int_chain pas trouvé");
    }
}

// For the cloud chain

void cpush(cloud_chain *l, tcloud k)
{
    assertAndLog(l != NULL, "cpush: l non initialisée");
    cloud_chain *m = malloc(sizeof(cloud_chain));
    m->data = k;
    m->next = l->next;
    l->next = m;
}

bool cis_empty(cloud_chain *l)
{
    assertAndLog(l != NULL, "cis_empty: l non initialisée");
    return (l->next == NULL);
}

cloud_chain *ccreate_list()
{
    /* Creer une liste vide */
    cloud_chain *l = malloc(sizeof(cloud_chain));
    tcloud k = {.coord = {.i = -1, .j = -1}, .pba = -1};
    l->data = k;
    l->next = NULL;
    return l;
}

tcloud cpop(cloud_chain *l)
{
    /* Supprime et renvoie le int_chain en première position */
    assert(!cis_empty(l));
    cloud_chain *m = l->next;
    l->next = m->next;
    tcloud k = m->data;
    free(m);
    return k;
}

void cfree(cloud_chain *l)
{
    while (!cis_empty(l))
    {
        cpop(l);
    }
    free(l);
}

// For all info pawns

void dpush(data_chain *l, pawn_info data)
{
    assertAndLog(l != NULL, "dpush : data_chain non initialisée");

    data_chain *m = malloc(sizeof(data_chain));
    m->data = data;
    m->next = l->next;
    l->next = m;
}
bool dis_empty(data_chain *l)
{
    assertAndLog(l != NULL, "dis_empty : data_chain non initialisée");
    return (l->next == NULL);
}

pawn_info dpop(data_chain *l)
{
    assertAndLog(l != NULL, "dpop : data_chain non initialisée");

    assert(!dis_empty(l));
    data_chain *m = l->next;
    l->next = m->next;
    pawn_info k = m->data;
    free(m);
    return k;
}

data_chain *dcreate_list()
{
    data_chain *l = malloc(sizeof(data_chain));
    pawn_info data_set = {.relationship = {.friendsId = NULL, .pos_foe = {.i = -1, .j = -1}, .queen = false},
                          .coord = {.i = -1, .j = -1}};
    l->data = data_set;
    l->next = NULL;
    return l;
}

void dfree(data_chain *l)
{
    while (!dis_empty(l))
    {
        dpop(l);
    }
    free(l);
}

// Opération pour manipuler un tableau de coordonnées dynamique

coord_tab_t *create_coord_tab(int size)
{
    assert(size > 0);
    coord_tab_t *t = malloc(sizeof(coord_tab_t));
    t->tab = malloc(size * sizeof(Coord));
    t->sizetab = size;
    t->index_actu = -1;
    for (int i = 0; i < size; i++)
    {
        t->tab[i].i = -1;
        t->tab[i].j = -1;
    }
    return t;
}

bool ValidCoordTabIndex(coord_tab_t *t, int i)
{
    return t->tab != NULL && t->sizetab > 0 && 0 <= i && i <= t->sizetab;
}

// ct means CoordTab

Coord ctget(coord_tab_t *t, int i)
{
    assert(ValidCoordTabIndex(t, i));
    return t->tab[i];
}

void ctset(coord_tab_t *t, int ind, int i, int j)
{
    assert(ValidCoordTabIndex(t, ind));
    t->tab[ind].i = i;
    t->tab[ind].j = j;
}

void ctfree(coord_tab_t *t)
{
    free(t->tab);
    free(t);
}

Coord ctpop(coord_tab_t *t)
{
    Coord pos = ctget(t, t->index_actu);
    ctset(t, t->index_actu, -1, -1);
    t->index_actu--;
    return pos;
}

void ctpush(coord_tab_t *t, int i, int j)
{
    t->index_actu++;
    ctset(t, t->index_actu, i, j);
}

bool ctis_empty(coord_tab_t *t)
{
    return t->index_actu == -1;
}

// For the other Kchain_list

void pushK(Kint_chain *l, int indPawnMoved, int indPawnMovedBack, int indPawnFriendlyMoved,
           int indPawnEnnemyMoved, int indQueenPromoted, int indQueenMoved)
{
    Kint_chain *m = malloc(sizeof(Kint_chain));
    m->indices.indPawnMoved = indPawnMoved;
    m->indices.indPawnMovedBack = indPawnMovedBack;
    m->indices.indPawnEnnemyMoved = indPawnFriendlyMoved;
    m->indices.indPawnEnnemyMoved = indPawnMoved;
    m->indices.indQueenPromoted = indQueenPromoted;
    m->indices.indQueenMoved = indQueenMoved;
    m->next = l->next;
    l->next = m;
}

bool is_emptyK(Kint_chain *l)
{
    return (l->next == NULL);
}

k_uplet popK(Kint_chain *l)
{
    assert(!is_emptyK(l));
    Kint_chain *m = l->next;
    l->next = m->next;
    k_uplet k = m->indices;
    free(m);
    return k;
}

Kint_chain *create_listK()
{
    Kint_chain *l = malloc(sizeof(Kint_chain));
    l->indices.indPawnMoved = -1;
    l->indices.indPawnMovedBack = -1;
    l->indices.indPawnEnnemyMoved = -1;
    l->indices.indPawnEnnemyMoved = -1;
    l->indices.indQueenPromoted = -1;
    l->indices.indQueenMoved = -1;
    l->next = NULL;
    return l;
}