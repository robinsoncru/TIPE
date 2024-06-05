#ifndef LITTLE_LINKED_LIST
#define LITTLE_LINKED_LIST

#include "coord.h"

void push(int_chain *l, int k);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool is_empty(int_chain *l);

int pop(int_chain *l);
/* Supprime et renvoie le int_chain en première position */

int get(int_chain *l, int i);
/* Renvoie le i-eme maillon */

void seti(int_chain *l, int i, int valeur);

void popi(int_chain *l, int ind);


int taille_list(int_chain *l);

int_chain *create_list(int size_max);
/* Creer une liste vide */

void replaceValueInList(int_chain *l, int old_value, int new_value);

void cpush(cloud_chain *l, tcloud k);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool cis_empty(cloud_chain *l);

tcloud cpop(cloud_chain *l);
/* Supprime et renvoie le int_chain en première position */

// tcloud cget(int_chain *l);
/* Renvoie le premier int_chain */

cloud_chain *ccreate_list();
/* Creer une liste vide */

void cfree(cloud_chain *l);

void dpush(data_chain *l, pawn_info data);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool dis_empty(data_chain *l);

pawn_info dpop(data_chain *l);
/* Supprime et renvoie le int_chain en première position */

// tcloud cget(int_chain *l);
/* Renvoie le premier int_chain */

data_chain *dcreate_list();
/* Creer une liste vide */

void dfree(data_chain *l);

void pushK(Kint_chain *l, int indPawnMoved, int indPawnMovedBack, int indPawnFriendlyMoved,
           int indPawnEnnemyMoved, int indQueenPromoted, int indQueenMoved);

bool is_emptyK(Kint_chain *l);

k_uplet popK(Kint_chain *l);

Kint_chain *create_listK();
void freeIntChain(int_chain *l);
void emptyIntChain(int_chain *l);
bool alreadyInList(int_chain *l, int ind);

// Coord tab, permet de stocker les positions au lieu des indices, les positions
// sont des valeurs surs pour repérer les pions contrairement aux indices


coord_tab_t *create_coord_tab(int size);
bool ValidCoordTabIndex(coord_tab_t *t, int i);
Coord ctget(coord_tab_t *t, int i);
void ctset(coord_tab_t *t, int ind, int i, int j);
void ctfree(coord_tab_t *t);
Coord ctpop(coord_tab_t *t);
void ctpush(coord_tab_t *t, int i, int j);
bool ctis_empty(coord_tab_t *t);
int cttaille(coord_tab_t *t);

void dfilterCoordIndSpecial(data_chain *l, Coord coordSpecial);
#endif