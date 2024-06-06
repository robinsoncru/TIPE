#include "applyMoveAI.h"

memory_move_t *applyDeter(Game *g, Move coup)
{
    // passer les pointeurs pour legereté
    memory_move_t *mem;
    auto_put_index(g, coup.manipulatedPawn);
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
        mem = queenDeplDeter(g, coup.manipulatedPawn, coup.pos_dame, coup.rafleTree, coup.rafle, coup.type);
        break;

    case rafleType:
        mem = rafleDeter(g, coup.manipulatedPawn, coup.rafleTree, coup.rafle, coup.type);
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
    mem->coordMovePawn = coord_from_ind(g, mem->indMovePawn, mem->is_white);
    mem->indMovePawn = -1; // Sécurité pour être sur de ne pas se baser sur l'indice

    moveFree(coup);
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

void applyRecipIssue(Game *g, memory_move_t *mem, int index)
{
    cancelSelectedIssue(g, mem, index);
}

void applyRecipDeter(Game *g, memory_move_t *mem)
{
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
}