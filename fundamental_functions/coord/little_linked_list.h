#ifndef LITTLE_LINKED_LIST
#define LITTLE_LINKED_LIST

#include "coord.h"

/* Victor G: j'ai implémenter une petite structure de liste chainee pour plus tard pour une fonction */

typedef struct maillon maillon;

struct maillon
{
    int ind;
    maillon *next;
};
/* Liste chainée contenant les indices des pions amis à faire bouger pour le joueur actuel */

void push(maillon *l, int k);
/* le maillon l est le maillon de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau maillon */

bool is_empty(maillon *l);

int pop(maillon *l);
/* Supprime et renvoie le maillon en première position */

int get(maillon *l);
/* Renvoie le premier maillon */

maillon *create_list();
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
/* le maillon l est le maillon de tête, on l'utilise pour accéder à la structure et y ajouter le nouveau maillon */

bool cis_empty(cloud_chain *l);

tcloud cpop(cloud_chain *l);
/* Supprime et renvoie le maillon en première position */

// tcloud cget(maillon *l);
/* Renvoie le premier maillon */

cloud_chain *ccreate_list();
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

typedef struct Kmaillon Kmaillon;

struct Kmaillon
{
    k_uplet indices;
    Kmaillon *next;
};

void pushK(Kmaillon *l, int indPawnMoved, int indPawnMovedBack, int indPawnFriendlyMoved,
           int indPawnEnnemyMoved, int indQueenPromoted, int indQueenMoved);

bool is_emptyK(Kmaillon *l);

k_uplet popK(Kmaillon *l);

Kmaillon *create_listK();

#endif