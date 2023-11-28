#!/usr/bin/bash

#for informations about this script, go to:
#https://www.freecodecamp.org/news/bash-scripting-tutorial-linux-shell-script-and-command-line-for-beginners

#a bash script that'll simply compile the code in a file named "dames.out"
#ce sera beaucoup plus simple pour compiler et tester le code :)

files_array=("main.c"
             "../fundamental_functions/coord/coord.c"
             "../fundamental_functions/game_functions/game_functions.c"
             "../graphic_interface/actions/input_command.c"
             "../graphic_interface/actions/on_input.c"
             "../graphic_interface/interface/misc/misc.c"
             "../graphic_interface/memory_update/graphic_cache.c"
             "../graphic_interface/memory_update/update.c"
             "../quantum_rules/quantum_functions.c"
             "../rafle/calc/path_tree.c"
             "../rafle/calc/rafle_tree_calc.c")

echo "Compilation en cours..."

gcc ${files_array[*]} $(sdl2-config --cflags --libs) -lSDL2_ttf -o dames.out

echo "Code compilé"
echo "Pour exécuter, tappez la commande"
echo "./dames.out"