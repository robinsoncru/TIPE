#ifndef FLISTE_CHAINE
#define FLISTE_CHAINE

#include "fcoord_struct.h"
/* Liste chainée contenant les indices des pions amis à faire bouger pour le joueur actuel */



typedef struct {
    Coord coord;
    int pba;
} tcloud;

typedef struct cloud_chain cloud_chain;

struct cloud_chain {
    tcloud data;
    cloud_chain *next;
};


typedef struct {
    primary_data_t relationship;
    Coord coord;
} pawn_info;

typedef struct data_chain data_chain;

struct data_chain {
    pawn_info data;
    data_chain *next;
};



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


#endif