#include "alpha_beta_prunning.h"
#include "alpha_beta_arg_struct.h"
#include "esperance_calc/esperance_calc.h"
#include "moveTab_quick_sort/moveTab_quick_sort.h"
#include <math.h>
#include <stdlib.h>

double alphaBetaMin(alphaBetaArg arg);

double alphaBetaMax(alphaBetaArg arg)
{
    if (arg.depth <= 0)
    {
        double perspective = arg.g->is_white ? 1 : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab *moveTab = listMovesNoRafleNoPromotion(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    double esperance;
    double max = -INFINITY;
    arg.depth--;
    int i;
    for (i = 0; i < tabSize; i++)
    {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin,
                                               arg, moveTab->tab[i], true);
        if (max < esperance)
        {
            max = esperance;
        }
        if (max >= arg.beta)
        {
            moveTabFreeTrees(moveTab, 0, tabSize);
            moveTabFree(moveTab, i + 1, tabSize);
            return max; // coupure beta
        }
    }
    moveTabFreeTrees(moveTab, 0, tabSize);
    return max;
}

double alphaBetaMin(alphaBetaArg arg)
{
    if (arg.depth <= 0)
    {
        double perspective = arg.g->is_white ? 1 : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab *moveTab = listMovesNoRafleNoPromotion(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    double esperance;
    double min = INFINITY;
    arg.depth--;
    int i;
    for (i = 0; i < tabSize; i++)
    {
        arg.beta = min;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMax,
                                               arg, moveTab->tab[i], true);
        if (min > esperance)
        {
            min = esperance;
        }
        if (min <= arg.alpha)
        {
            moveTabFreeTrees(moveTab, 0, tabSize);
            moveTabFree(moveTab, i + 1, tabSize);
            return min; // coupure alpha
        }
    }
    moveTabFreeTrees(moveTab, 0, tabSize);
    return min;
}

// MoveTab *filterRafleNoQueen(MoveTab *moveTab)
// {
//     int nv_taille = 0;
//     for (int i = 0; i < moveTab->size; i++)
//     {
//         if (moveTab->tab[i].type != rafleType)
//         {
//             nv_taille++;
//         }
//     }
//     MoveTab *newMoveTab = malloc(sizeof(moveTab));
//     newMoveTab->tab = malloc(nv_taille * sizeof(Move));
//     newMoveTab->size = nv_taille;
//     int ind=0;
//     for (int i=0; i<moveTab->size; i++){
//         if (moveTab->tab[i].type != rafleType)
//         {
//             newMoveTab->tab[ind] = moveTab->tab[i];
//         }
//     }

//     return newMoveTab;
// }

Move alphaBetaPrunning(Game *g, AI ai)
{
    // simule le calcul du meilleur coup pour le joueur
    // dont c'est le tour selon l'IA en argument

    alphaBetaArg arg;
    arg.beta = INFINITY;
    arg.ai = ai;
    arg.depth = ai.depth;
    arg.g = g;

    MoveTab *moveTab = listMovesNoRafleNoPromotion(g);

    print_moves_write_file_while_endgame(g, moveTab);
    print_state_game(g);
    moveTabQuickSort(g, moveTab, ai);

    int tabSize = moveTab->size;
    double esperance;
    double max = -INFINITY;
    int bestMoveIndex;
    arg.depth--;
    for (int i = 0; i < tabSize; i++)
    {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin, arg,
                                               moveTab->tab[i], false);
        if (max <= esperance)
        {
            max = esperance;
            bestMoveIndex = i;
        }
    }
    // libere tout le monde sauf le move qu'on retourne
    // faudra le liberer apres l'avoir applique
    Move bestMove = moveTab->tab[bestMoveIndex];
    Move currentMove;
    PathTree *currentTree;
    switch (bestMove.type)
    {
    case rafleType:
        for (int i = 0; i < tabSize; i++)
        {
            currentTree = moveTab->tab[i].rafleTree;
            if (currentTree != bestMove.rafleTree)
            {
                pathTreeFree(currentTree);
            }
        }

        for (int i = 0; i < bestMoveIndex; i++)
        {
            pathFree(moveTab->tab[i].rafle);
        }
        for (int i = bestMoveIndex + 1; i < tabSize; i++)
        {
            pathFree(moveTab->tab[i].rafle);
        }
        break;

    case pawnMoveBackType:
        for (int i = 0; i < bestMoveIndex; i++)
        {
            currentMove = moveTab->tab[i];
            free(currentMove.backwardPawnMoves->tab);
            free(currentMove.backwardPawnMoves);
        }
        for (int i = bestMoveIndex + 1; i < tabSize; i++)
        {
            currentMove = moveTab->tab[i];
            free(currentMove.backwardPawnMoves->tab);
            free(currentMove.backwardPawnMoves);
        }
        break;

    default:
        break;
    }
    free(moveTab->tab);
    free(moveTab);
    return bestMove;
}