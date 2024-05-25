#include "print_moves.h"

void print_move(Move m)
{
    printf("--------------\n");
    printf("Pièce: %d\n", m.manipulatedPawn);
    switch (m.type)
    {
    case pawnMoveType:
        if (m.left)
        {
            printf("^ <-\n");
        }
        else
        {
            printf("^ ->\n");
        }
        break;

    case promotionType:
        printf("Prom\n");

    case pawnMoveBackType:
        if (m.left)
        {
            printf("v <-\n");
        }
        else
        {
            printf("v ->\n");
        }
        break;

    case biDeplType:
        printf("<- ->\n");
        break;

    case queenDeplType:
        printf("~>\n");
        printf("Arrive Dame {%d, %d}\nPièce mangée {%d, %d}\n", m.coords.pos_dame.i, m.coords.pos_dame.j,
               m.coords.pos_eaten_pawn.i, m.coords.pos_eaten_pawn.j);
        break;

    case rafleType:
        printf("<< miam\n");
        break;

    case lienAmitieType:
        printf(":) ");
        printf("Pièce ami : {%d, %d}\n", m.lig, m.col);
        break;

    case lienEnnemitieType:
        printf(":( ");
        printf("Pièce ennemie : {%d, %d}\n", m.lig, m.col);
        break;

    default:
        assertAndLog(false, "aucun coup précalculé reconnu à afficher");
        break;
    }
    printf("--------------\n\n");
    flush();
}

void print_moves(MoveTab *t)
{
    for (int i = 0; i < t->size; i++)
    {
        print_move(t->tab[i]);
    }
}