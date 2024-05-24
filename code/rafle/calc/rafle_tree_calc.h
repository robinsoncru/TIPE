#include <stdbool.h>
#include <stdio.h>
#include "path_tree.h"
#include "../../fundamental_functions/game_functions/aux_game_functions/aux_functions.h"
#include "../../fundamental_functions/game_functions/precondition/precondition.h"
#include "../../fundamental_functions/game_functions/access_functions/access_functions.h"

PathTree* rafleTreeCalc(Game *g, bool color, int indSerialKiller);