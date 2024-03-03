#include "play_move.h"

memory_move_t *applyDeter(Game *g, Move coup)
{
    // passer les pointeurs pour legereté
    memory_move_t *mem;
    switch (coup.type)
    {
    case pawnMoveType:
        mem = issuePawnMove(g, coup.manipulatedPawn, coup.left, coup.type);
        break;

    case promotionType:
        mem = issuePromotion(g, coup.manipulatedPawn, coup.type);
        break;

    case pawnMoveBackType:
        mem = issueMoveBack(g, coup.manipulatedPawn, coup.left, coup.type);
        break;

    case biDeplType:
        mem = issueDeter(coup.manipulatedPawn, coup.type);
        break;

    case queenDeplType:
        mem = issueQueenDepl(g, coup.manipulatedPawn, coup.coords, coup.rafleTree, coup.rafle, coup.type);
        break;

    case rafleType:
        mem = issueRafle(g, coup.manipulatedPawn, coup.rafleTree, coup.rafle, coup.type);
        break;

    case lienAmitieType:
    case lienEnnemitieType:
        mem = issueDeter(coup.manipulatedPawn, coup.type);
        mem->lig =coup.lig;
        mem->col = coup.col;
        break;

    default:
        assertAndLog(true, "reconnait pas type move");
        mem = NULL;
        break;
    }

    return mem;
}

void applyMove(Game *g, memory_move_t *mem, int nbIssue)
{
    switch (mem->type)
    {
    case pawnMoveType:
        pawnMoveAI(g, mem, nbIssue);
        break;

    case promotionType:
        promotionAI(g, mem, nbIssue);
        break;

    case pawnMoveBackType:
        pawnMoveBackAI(g, mem, nbIssue);
        break;

    case biDeplType:
        biDeplAI(g, nbIssue, mem);
        break;

    case queenDeplType:
        queenDeplAI(g, mem, nbIssue);
        break;

    case rafleType:
        rafleAI(g, mem, nbIssue);
        break;

    case lienAmitieType:
        lienAmitieAI(g, mem->indMovePawn, mem->lig, mem->col, mem);
        break;

    case lienEnnemitieType:
        lienEnnemitieAI(g, mem->indMovePawn, mem->lig, mem->col, mem);
        break;

    default:
        assertAndLog(true, "reconnait pas type move");
        break;
    }
}

void applyRecipMove(Game *g, memory_move_t *mem)
{
    switch (mem->type)
    {
    case promotionType:
        cancelIssuePromotion(g, mem);
        break;

    default:
    cancelSelectedIssue(g, mem);
        break;
    }
}

void applyRecipDeter(Game *g, memory_move_t *mem) {
    switch (mem->type)
    {
    case pawnMoveType:
        cancelPawnMoveAI(g, mem);
        break;

    case promotionType:
        cancelPromotionAI(g, mem);
        break;

    case pawnMoveBackType:
        cancelPawnMoveBackAI(g, mem);
        break;

    case biDeplType:
        cancelBiDeplAI(g, mem);
        break;

    case queenDeplType:
        cancelQueenDeplAI(g, mem);
        break;

    case rafleType:
        cancelRafleAI(g, mem);
        break;

    case lienAmitieType:
        cancelLienAmitieAI(g, mem);
        break;

    case lienEnnemitieType:
        cancelLienEnnemitieAI(g, mem);
        break;

    default:
        assertAndLog(true, "reconnait pas type move");
        break;
    }
}