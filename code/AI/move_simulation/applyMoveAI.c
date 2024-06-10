#include "applyMoveAI.h"

memory_move_t *applyDeter(Game *g, Move coup, bool shouldFreeMove)
{
    // passer les pointeurs pour legereté
    memory_move_t *mem;
    int ind_actu_coup_a_jouer = ind_from_coord(g, coup.coordManipulatedPawn);
    auto_put_index(g, ind_actu_coup_a_jouer);
    // On utilise parfois g dans les fonctions Deter

    switch (coup.type)
    {
    case pawnMoveType:
        mem = pawnMoveDeter(g, coup.manipulatedPawn, coup.left, coup.type);
        break;

    case promotionType:
        mem = promotionDeter(g, coup.manipulatedPawn, coup.type);
        break;

    case pawnMoveBackType:
        mem = moveBackDeter(g, coup.type, coup.backwardPawnMoves);
        break;

    case biDeplType:
        mem = biDeplDeter(g, coup.manipulatedPawn, coup.type);
        break;

    case queenDeplType:
        mem = queenDeplDeter(g, coup.manipulatedPawn, coup.pos_dame, coup.type);
        break;

    case rafleType:
        mem = rafleDeter(g, coup.manipulatedPawn, coup.rafleTree, coup.rafle, coup.type, coup.pos_dame);
        break;

    case lienAmitieType:
    case lienEnnemitieType:
        mem = initMemMove(coup.manipulatedPawn, coup.type);
        mem->lig = coup.lig;
        mem->col = coup.col;
        break;

    default:

        mem = NULL;
        assertAndLog(false, "Deter : reconnait pas type move");
        break;
    }
    mem->is_white = g->is_white;
    mem->movePawn.coord = coord_from_ind(g, mem->indMovePawn, mem->is_white);
    mem->indMovePawn = -1; // Sécurité pour être sur de ne pas se baser sur l'indice

    if (shouldFreeMove)
    {
        moveFreeBackwardMoveTab(coup);
    }
    // print_data_chain(mem->chainy);
    return mem;
}

void applyIssue(Game *g, memory_move_t *mem, int nbIssue)
{
    switch (mem->type)
    {
    case pawnMoveType:
        pawnMoveIssue(g, mem, nbIssue);
        break;

    case promotionType:
        promotionIssue(g, mem, nbIssue);
        break;

    case pawnMoveBackType:
        pawnMoveBackIssue(g, mem, nbIssue);
        break;

    case biDeplType:
        biDeplIssue(g, nbIssue, mem);
        break;

    case queenDeplType:
        queenDeplIssue(g, mem, nbIssue);
        break;

    case rafleType:
        rafleIssue(g, mem, nbIssue);
        break;

    case lienAmitieType:
        lienAmitieIssue(g, mem->indMovePawn, mem->lig, mem->col, mem);
        break;

    case lienEnnemitieType:
        lienEnnemitieIssue(g, mem->indMovePawn, mem->lig, mem->col, mem);
        break;

    default:
        assertAndLog(false, "Issue : reconnait pas type move");
        break;
    }
}

void cancelPromQueenPromoted(Game *g, cloud_queen_t *pawn, bool color)
{
    if (validCoord(pawn->coord) && pawn->had_become_queen)
    {
        int ind = ind_from_coord(g, pawn->coord);
        put_pawn_value(g, color, ind, QUEEN, 0);
        pawn->had_become_queen = false;
    }
}

void applyRecipIssue(Game *g, memory_move_t *mem, int index)
{
    bool color = mem->is_white;
    cancelPromQueenPromoted(g, &mem->movePawn, color);
    if (!mem->is_deter)
    {
        cancelPromQueenPromoted(g, &mem->pawnCloudOtherColor, !color);
        cancelPromQueenPromoted(g, &mem->pawnCloudSameColor, color);
        if (mem->type == biDeplType)
        {
            cancelSelectedIssueBiDepl(g, mem, index);
        }
        else
        {
            cancelSelectedIssue(g, mem, index);
        }
    }
}

void applyRecipDeter(Game *g, memory_move_t *mem)
{
    // On annule d'abord la promotion des pièces pion joué et survivants de nuages

    switch (mem->type)
    {
    case pawnMoveType:
        cancelPawnMoveDeter(g, mem);
        break;

    case promotionType:
        cancelPromotionDeter(g, mem);
        break;

    case pawnMoveBackType:
        cancelPawnMoveBackDeter(g, mem);
        break;

    case biDeplType:
        cancelBiDeplDeter(g, mem);
        break;

    case queenDeplType:
        cancelQueenDeplDeter(g, mem);
        break;

    case rafleType:
        cancelRafleDeter(g, mem);
        break;

    case lienAmitieType:
        cancelLienAmitieDeter(g, mem);
        break;

    case lienEnnemitieType:
        cancelLienEnnemitieDeter(g, mem);
        break;

    default:
        assertAndLog(false, "reconnait pas type move");
        break;
    }
    freeMemMove(mem);
}