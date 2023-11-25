#!/usr/bin/bash

#for informations about this script, go to:
#https://www.freecodecamp.org/news/bash-scripting-tutorial-linux-shell-script-and-command-line-for-beginners

#a bash script that'll simply compile the code in a file named "dames.out"
#ce sera beaucoup plus simple pour compiler et tester le code :)

files_array=("main.c"
             "../fundamental_functions/coord/coord.c"
             "../fundamental_functions/coord/little_linked_list.c"
             "../fundamental_functions/game_functions/game_functions.c"
             "../fundamental_functions/game_functions/aux_game_functions/aux_functions.c"
             "../fundamental_functions/game_functions/debug_and_management/debug_functions.c"
             "../fundamental_functions/game_functions/debug_and_management/game_management.c"
             "../fundamental_functions/game_functions/Logic_and_calculus/calculus_functions.c"
             "../fundamental_functions/game_functions/Logic_and_calculus/logic_functions.c"
             "../fundamental_functions/interface/interface.c"
             "../graphic_interface/actions/input_command.c"
             "../graphic_interface/actions/on_input.c"
             "../graphic_interface/memory_update/graphic_cache.c"
             "../graphic_interface/memory_update/update.c"
             "../rafle/calc/path_tree.c"
             "../rafle/calc/rafle_tree_calc.c"
             "../rafle/calc/test.c"
             "../rafle/display/path_tree_display.c"
             "../rafle/display/rafle_management.c")

echo "Compilation en cours..."

gcc ${files_array[*]} $(sdl2-config --cflags --libs) -lSDL2_ttf -o dames.out

echo "Code compilé"
echo "Pour exécuter, tappez la commande"
echo "./dames.out"
