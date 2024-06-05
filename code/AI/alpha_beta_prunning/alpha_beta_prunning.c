#include "alpha_beta_prunning.h"
#include "alpha_beta_arg_struct.h"
#include "esperance_calc/esperance_calc.h"
#include "moveTab_quick_sort/moveTab_quick_sort.h"
#include <math.h>

float alphaBetaMin(alphaBetaArg arg);

float alphaBetaMax(alphaBetaArg arg){
    if (arg.depth <= 0) {
        float perspective = arg.g->is_white ? 1  : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab* moveTab = listMoves(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    float esperance;
    float max = -INFINITY;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin,
            arg, moveTab->tab[i]);
        if (max < esperance) {
            max = esperance;
        }
        if (max >= arg.beta) {
            return max; //coupure beta
        }
    }
    return max;
}

float alphaBetaMin(alphaBetaArg arg){
    if (arg.depth <= 0) {
        float perspective = arg.g->is_white ? 1 : -1;
        return arg.ai.ecrasement(perspective * arg.ai.analyse(arg.g));
    }
    MoveTab* moveTab = listMoves(arg.g);
    moveTabQuickSort(arg.g, moveTab, arg.ai);

    int tabSize = moveTab->size;
    float esperance;
    float min = INFINITY;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.beta = min;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMax,
        arg, moveTab->tab[i]);
        if (min > esperance) {
            min = esperance;
        }
        if (min <= arg.alpha) {
            return min; //coupure alpha
        }
    }
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
    float esperance;
    float max = -INFINITY;
    Move bestMove;
    arg.depth--;
    for (int i = 0; i < tabSize; i++) {
        arg.alpha = max;
        esperance = esperanceAlphaBetaPrunning(alphaBetaMin, arg, moveTab->tab[i]);
        if (max <= esperance) {
            max = esperance;
            bestMove = moveTab->tab[i];
        }
    }
    return bestMove;
}