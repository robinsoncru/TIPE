#include <stdlib.h>
#include <stdbool.h>
#include "rafle_tree_calc/path_tree.h"

//La nouvelle structure de rafle sera composee de deux sous-structures :
//D'abord, un arbre de rafles, et d'autre part une file d'entiers entre 0 et 3 inclus
//symbolisants une direction a prendre a chaque embranchement

typedef struct Path Path;