#include "applyMoveForSure.h"

void applyForSure(Game *g, GraphicCache *cache, Move coup)
{
    // passer les pointeurs pour legereté
    int ind = coup.manipulatedPawn;
    bool color = g->is_white;
    auto_put_index(g, ind);
    // On utilise parfois g dans les fonctions Deter

    switch (coup.type)
    {
    case pawnMoveType:
        checkPawnMove(g, cache, coup.left, true);
        break;

    case promotionType:
        checkPromotion(g, cache, true);
        break;

    case pawnMoveBackType:
        moveBack(g, true, false, zero_fun);
        break;

    case biDeplType:
        checkBiDepl(g, cache, true);
        break;

    case queenDeplType:
        queenDeplDueMoveStruct(g, ind, color, coup.pos_dame);
        break;

    case rafleType:
        rafleForSure(g, cache, color, ind, coup.rafleTree, coup.rafle, coup.pos_dame);
        break;

    case lienAmitieType:
        checkLienAmitie(coup.lig, coup.col, g, cache, false);
        break;

    case lienEnnemitieType:
        checkLienEnnemitie(coup.lig, coup.col, g, cache, false);
        break;

    case passType:
        //il y a effectivement des cas ou on ne peut pas jouer
        break;

    default:
        assertAndLog(false, "Apply for sure : reconnait pas type move");
        break;
    }
    // moveFree(coup);
}