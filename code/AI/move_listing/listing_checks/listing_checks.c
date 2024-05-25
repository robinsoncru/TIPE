#include "listing_checks.h"

bool isFriendable(Game *g, int selectedPawn, bool is_white)
{
    return int_to_bool(get_pawn_value(g, is_white, selectedPawn, ALIVE)) && get_pawn_value(g, is_white, selectedPawn, ENNEMY) == VOID_INDEX && get_pawn_value(g, is_white, selectedPawn, PBA) == 1 && !int_to_bool(get_pawn_value(g, is_white, selectedPawn, QUEEN));
}

bool isEnnemiable(Game *g, int selectedPawn, bool is_white)
{
    return int_to_bool(get_pawn_value(g, is_white, selectedPawn, ALIVE)) && !int_to_bool(get_pawn_value(g, is_white, selectedPawn, QUEEN)) && get_pawn_value(g, is_white, selectedPawn, ENNEMY) == VOID_INDEX && !int_to_bool(get_pawn_value(g, is_white, selectedPawn, FRIENDLY)) && get_pawn_value(g, is_white, selectedPawn, PBA) == 1;
}

bool isPromotable(Game *g, int selectedPawn, bool is_white)
{
    return int_to_bool(get_pawn_value(g, is_white, selectedPawn, ALIVE)) && !int_to_bool(get_pawn_value(g, is_white, selectedPawn, QUEEN)) && get_pawn_value(g, is_white, selectedPawn, PBA) == 1;
}