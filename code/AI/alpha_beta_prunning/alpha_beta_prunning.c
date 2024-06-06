#include "alpha_beta_prunning.h"
#include "alpha_beta_arg_struct.h"
#include "esperance_calc/esperance_calc.h"
#include "moveTab_quick_sort/moveTab_quick_sort.h"
#include <math.h>

double alphaBetaMin(alphaBetaArg arg);

double alphaBetaMax(alphaBetaArg arg){
    if (arg.depth <= 0) {
        double perspective = arg.g->is_white ? 1  : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab* moveTab = listMoves(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    double esperance;
    double max = -INFINITY;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin,
            arg, moveTab->tab[i], true);
        if (max < esperance) {
            max = esperance;
        }
        if (max >= arg.beta) {
            return max; //coupure beta
        }
    }
    moveTabFree(moveTab);
    return max;
}

double alphaBetaMin(alphaBetaArg arg){
    if (arg.depth <= 0) {
        double perspective = arg.g->is_white ? 1 : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab* moveTab = listMoves(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    double esperance;
    double min = INFINITY;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.beta = min;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMax,
        arg, moveTab->tab[i], true);
        if (min > esperance) {
            min = esperance;
        }
        if (min <= arg.alpha) {
            return min; //coupure alpha
        }
    }
    moveTabFree(moveTab);
    return min;
}

Move alphaBetaPrunning(Game *g, AI ai){
    //simule le calcul du meilleur coup pour le joueur
    //dont c'est le tour selon l'IA en argument

    alphaBetaArg arg;
    arg.beta = INFINITY;
    arg.ai = ai;
    arg.depth = ai.depth;
    arg.g = g;

    MoveTab* moveTab = listMoves(g);
    moveTabQuickSort(g, moveTab, ai);

    int tabSize = moveTab->size;
    double esperance;
    double max = -INFINITY;
    int bestMoveIndex;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin, arg,
            moveTab->tab[i], false);
        if (max <= esperance) {
            max = esperance;
            bestMoveIndex = i;
        }
    }
    //libere tout le monde sauf le move qu'on retourne
    //faudra le liberer apres l'avoir applique
    for (int i = 0; i < bestMoveIndex; i++) {
        moveFree(moveTab->tab[i]);
    }
    for (int i = bestMoveIndex + 1; i < tabSize; i++) {
        moveFree(moveTab->tab[i]);
    }
    return moveTab->tab[bestMoveIndex];
}