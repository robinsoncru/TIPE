#include "esperance_calc.h"
#include "../../move_simulation/applyMoveAI.h"
#include "../../../structures_fondamentales/fmemory.h"

double getProba(issue_t outcome){
    int probaDueToGhostPawn = (outcome.pba >= 0) ? outcome.pba : 1;
    int probaDueToPromotion = (outcome.pba_promotion >= 0) ? outcome.pba_promotion : 1;
    return 1. / ((double) (probaDueToPromotion * probaDueToGhostPawn));
}

double esperanceAlphaBetaPrunning(double (*f)(alphaBetaArg), alphaBetaArg alphaBetaArg, Move move, bool shouldFreeMove){
    memory_move_t* mem = applyDeter(alphaBetaArg.g, move, shouldFreeMove);

    double S = 0;
    double currentProba;
    int nbOutcomes = mem->lenghtIssues;
    issue_t* outcomeTab = mem->issues;
    switch (nbOutcomes) {
        case 1:
            S += f(alphaBetaArg);
            break;
        
        default:
            for (int i = 0; i < nbOutcomes; i++) {
                applyIssue(alphaBetaArg.g, mem, i);
                currentProba = getProba(outcomeTab[i]);
                S += currentProba * f(alphaBetaArg);
                applyRecipIssue(alphaBetaArg.g, mem, i);
            }
            break;
    }

    applyRecipDeter(alphaBetaArg.g, mem);
    return S;
}

double esperanceHeuristique(AI ai, Game* g, Move move){
    double perspective = g->is_white ? 1 : -1;
    memory_move_t* mem = applyDeter(g, move, false);
    // print_state_game(g);
    double S = 0;
    double currentProba;
    int nbOutcomes = mem->lenghtIssues;
    issue_t* outcomeTab = mem->issues;

    switch (nbOutcomes) {
    case 1:
        S+= ai.ecrasement(perspective * ai.analyse(g));
        break;
    
    default:
        for (int i = 0; i < nbOutcomes; i++) {
            // print_state_game(g);
            applyIssue(g, mem, i);
            currentProba = getProba(outcomeTab[i]);
            S += currentProba * ai.ecrasement(perspective * ai.analyse(g));
            applyRecipIssue(g, mem, i);
            // print_state_game(g);
        }
        break;
    }

    // print_state_game(g);
    applyRecipDeter(g, mem);
    // print_state_game(g);
    // printv("test");
    return S;
}