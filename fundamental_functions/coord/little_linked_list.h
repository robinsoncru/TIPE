#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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

maillon *create_list();
/* Creer une liste vide */