dames.out: main.o coord.o little_linked_list.o game_functions.o aux_functions.o debug_functions.o game_management.o \
calculus_functions.o logic_functions.o play_back_game_functions.o interface.o input_command.o on_input.o graphic_cache.o update.o \
path_tree.o rafle_tree_calc.o path_tree_display.o rafle_management.o four_pack.o path_struct.o rafle_eating.o move_simulation.o \
move_max.o move_listing.o
	gcc -g -Wall -o dames.out main.o coord.o little_linked_list.o game_functions.o aux_functions.o debug_functions.o game_management.o \
calculus_functions.o logic_functions.o play_back_game_functions.o interface.o input_command.o on_input.o graphic_cache.o update.o \
path_tree.o rafle_tree_calc.o path_tree_display.o rafle_management.o four_pack.o path_struct.o rafle_eating.o move_simulation.o \
move_max.o move_listing.o \
$$(sdl2-config --cflags --libs) -lSDL2_ttf

main.o: main.c
	gcc -g -c -Wall main.c

coord.o: fundamental_functions/coord/coord.c
	gcc -g -c -Wall fundamental_functions/coord/coord.c

little_linked_list.o: fundamental_functions/coord/little_linked_list.c
	gcc -g -c -Wall fundamental_functions/coord/little_linked_list.c

game_functions.o: fundamental_functions/game_functions/game_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/game_functions.c

aux_functions.o: fundamental_functions/game_functions/aux_game_functions/aux_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/aux_game_functions/aux_functions.c

debug_functions.o: fundamental_functions/game_functions/debug/debug_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/debug/debug_functions.c

game_management.o: fundamental_functions/game_functions/management/game_management.c
	gcc -g -c -Wall fundamental_functions/game_functions/management/game_management.c

calculus_functions.o: fundamental_functions/game_functions/Logic_and_calculus/calculus_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/Logic_and_calculus/calculus_functions.c

logic_functions.o: fundamental_functions/game_functions/Logic_and_calculus/logic_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/Logic_and_calculus/logic_functions.c

play_back_game_functions.o: fundamental_functions/game_functions/play_back/play_back_game_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/play_back/play_back_game_functions.c

interface.o: fundamental_functions/interface/interface.c
	gcc -g -c -Wall fundamental_functions/interface/interface.c

input_command.o: graphic_interface/actions/input_command.c
	gcc -g -c -Wall graphic_interface/actions/input_command.c

on_input.o: graphic_interface/actions/on_input.c
	gcc -g -c -Wall graphic_interface/actions/on_input.c

graphic_cache.o: graphic_interface/memory_update/graphic_cache.c
	gcc -g -c -Wall graphic_interface/memory_update/graphic_cache.c

update.o: graphic_interface/memory_update/update.c
	gcc -g -c -Wall graphic_interface/memory_update/update.c

path_tree.o: rafle/calc/path_tree.c
	gcc -g -c -Wall rafle/calc/path_tree.c

rafle_tree_calc.o: rafle/calc/rafle_tree_calc.c
	gcc -g -c -Wall rafle/calc/rafle_tree_calc.c

path_tree_display.o: rafle/display/path_tree_display.c
	gcc -g -c -Wall rafle/display/path_tree_display.c

rafle_management.o: rafle/display/rafle_management.c
	gcc -g -c -Wall rafle/display/rafle_management.c

four_pack.o: rafle/rafle_choice/four_pack.c
	gcc -g -c -Wall rafle/rafle_choice/four_pack.c

path_struct.o: rafle/rafle_choice/path_struct.c
	gcc -g -c -Wall rafle/rafle_choice/path_struct.c

rafle_eating.o: rafle/rafle_eating/rafle_eating.c
	gcc -g -c -Wall rafle/rafle_eating/rafle_eating.c

move_simulation.o: AI/move_simulation/move_simulation.c
	gcc -g -c -Wall AI/move_simulation/move_simulation.c

move_max.o: AI/move_max/move_max.c
	gcc -g -c -Wall AI/move_max/move_max.c

move_listing.o: AI/move_listing/move_listing.c
	gcc -g -c -Wall AI/move_listing/move_listing.c

