#!/usr/bin/bash

#for informations about this script, go to:
#https://www.freecodecamp.org/news/bash-scripting-tutorial-linux-shell-script-and-command-line-for-beginners

#a bash script that'll simply compile the code in a file named "dames.out"
#ce sera beaucoup plus simple pour compiler et tester le code :)

files_array=("main.c"
             "fundamental_functions/interface_jeu_dames.c"
             "fundamental_functions/game_functions_draughts.c"
             "fundamental_functions/coord/coord.c"
             #"rafle/calc/rafle_calc.c"
             #"rafle/calc/rafle_struct.c"
             #"rafle/board_modifying/board_modifying.c"
             "new_rafle/calc/rafle_tree_calc/path_tree.c"
             "new_rafle/calc/rafle_tree_calc/rafle_tree_calc.c"
             "new_rafle/calc/rafle_tree_calc/test.c"
             "new_rafle/display/path_tree_display.c"
             "new_rafle/display/test_tree_display.c")

echo "Compilation en cours..."

gcc ${files_array[*]} $(sdl2-config --cflags --libs) -lSDL2_ttf -o dames.out

echo "Code compilé"
echo "Pour exécuter, tappez la commande"
echo "./dames.out"