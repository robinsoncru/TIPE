#ifndef LITTLE_LINKED_LIST
#define LITTLE_LINKED_LIST

#include "coord.h"

typedef struct int_chain int_chain;

struct int_chain
{
    int *tableau;
    int ind_actu;
    int size_max;
};
/* Liste chainée contenant les indices des pions amis à faire bouger pour le joueur actuel */

void push(int_chain *l, int k);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool is_empty(int_chain *l);

int pop(int_chain *l);
/* Supprime et renvoie le int_chain en première position */

int get(int_chain *l);
/* Renvoie le premier int_chain */

int_chain *create_list();
/* Creer une liste vide */


typedef struct {
    Coord coord;
    int pba;
} tcloud;

typedef struct cloud_chain cloud_chain;

struct cloud_chain {
    tcloud data;
    cloud_chain *next;
};

void cpush(cloud_chain *l, tcloud k);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool cis_empty(cloud_chain *l);

tcloud cpop(cloud_chain *l);
/* Supprime et renvoie le int_chain en première position */

// tcloud cget(int_chain *l);
/* Renvoie le premier int_chain */

cloud_chain *ccreate_list();
/* Creer une liste vide */

typedef struct {
    primary_data_t relationship;
    Coord coord;
} pawn_info;

typedef struct data_chain data_chain;

struct data_chain {
    pawn_info data;
    data_chain *next;
};

void dpush(data_chain *l, pawn_info data);
/* le int_chain l est le int_chain de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau int_chain */

bool dis_empty(data_chain *l);

pawn_info dpop(data_chain *l);
/* Supprime et renvoie le int_chain en première position */

// tcloud cget(int_chain *l);
/* Renvoie le premier int_chain */

data_chain *dcreate_list();
/* Creer une liste vide */


typedef struct k_uplet k_uplet;

struct k_uplet
{
    int indPawnMoved;
    int indPawnMovedBack;
    int indPawnFriendlyMoved;
    int indPawnEnnemyMoved;
    int indQueenPromoted;
    int indQueenMoved;
};

typedef struct Kint_chain Kint_chain;

struct Kint_chain
{
    k_uplet indices;
    Kint_chain *next;
};

void pushK(Kint_chain *l, int indPawnMoved, int indPawnMovedBack, int indPawnFriendlyMoved,
           int indPawnEnnemyMoved, int indQueenPromoted, int indQueenMoved);

bool is_emptyK(Kint_chain *l);

k_uplet popK(Kint_chain *l);

Kint_chain *create_listK();

#endif