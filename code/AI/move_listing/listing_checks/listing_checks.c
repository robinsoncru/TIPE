#include "listing_checks.h"

bool isFriendable(Game *g, int selectedPawn, bool is_white){
    pawn p = g->allPawns[is_white][selectedPawn];
    return p.alive && p.ennemy == VOID_INDEX && p.friendly == VOID_INDEX && p.pba == 1;
}

bool isEnnemiable(Game *g, int selectedPawn, bool is_white){
    pawn p = g->allPawns[is_white][selectedPawn];
    return p.alive && !p.queen && p.ennemy == VOID_INDEX && p.friendly == VOID_INDEX && p.pba == 1;
}

bool isPromotable(Game *g, int selectedPawn, bool is_white){
    pawn p = g->allPawns[is_white][selectedPawn];
    return p.alive && !p.queen && p.pba == 1;
}