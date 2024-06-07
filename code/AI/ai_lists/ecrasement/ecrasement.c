#include "ecrasement.h"
#include <math.h>
// double arctan(double x){
//     return atan(x);
// }

double halfOfSquareRootOfPiSup = .886226925452758013649083741670572591398774728061193565;
//la derniere deciamle est volontairement au dessus de la vrai valeur
//pour s'assurer que erf(inf) soit plus grand en valeur absolue que
//erf(x) pour x reel

int calcLim = 10;

double erf(double x){
    
    if (x == INFINITY) {
        return halfOfSquareRootOfPiSup;
    }
    if (x == -INFINITY) {
        return -halfOfSquareRootOfPiSup;
    }
    double y = -x*x;
    double multiplicativePartOfTerm = x;
    double S = x; //premier terme de la somme
    for (int i = 0; i < calcLim; i++) {
        multiplicativePartOfTerm *= y/((double) i + 1);
        S+= multiplicativePartOfTerm *  1./((double) 2 * i + 1);
    }
    return S;
}