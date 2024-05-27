dames.out: main.o coord.o little_linked_list.o game_functions.o aux_functions.o debug_functions.o game_management.o \
calculus_functions.o logic_functions.o play_back_game_functions.o affichage.o input_command.o on_input.o graphic_cache.o update.o \
path_tree.o rafle_tree_calc.o path_tree_display.o rafle_management.o four_pack.o path_struct.o rafle_eating.o move_simulation.o \
move_max.o move_listing.o listing_checks.o move_struct.o rafle_listing.o basic_play_function.o precondition.o access_functions.o \
print_moves.o
	gcc -g -Wall -o dames.out main.o coord.o little_linked_list.o game_functions.o aux_functions.o debug_functions.o game_management.o \
calculus_functions.o logic_functions.o play_back_game_functions.o affichage.o input_command.o on_input.o graphic_cache.o update.o \
path_tree.o rafle_tree_calc.o path_tree_display.o rafle_management.o four_pack.o path_struct.o rafle_eating.o move_simulation.o \
move_max.o move_listing.o listing_checks.o move_struct.o rafle_listing.o basic_play_function.o precondition.o access_functions.o \
print_moves.o debug_autoplay.o debug.o \
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

debug_functions.o: fundamental_functions/game_functions/debug_game_functions/debug_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/debug_game_functions/debug_functions.c

game_management.o: fundamental_functions/game_functions/management/game_management.c
	gcc -g -c -Wall fundamental_functions/game_functions/management/game_management.c

calculus_functions.o: fundamental_functions/game_functions/Calculus/calculus_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/Calculus/calculus_functions.c

access_functions.o: fundamental_functions/game_functions/access_functions/access_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/access_functions/access_functions.c

logic_functions.o: fundamental_functions/game_functions/Logic/logic_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/Logic/logic_functions.c

play_back_game_functions.o: fundamental_functions/game_functions/play_back/play_back_game_functions.c
	gcc -g -c -Wall fundamental_functions/game_functions/play_back/play_back_game_functions.c

affichage.o: fundamental_functions/affichage/affichage.c
	gcc -g -c -Wall fundamental_functions/affichage/affichage.c

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

move_max.o: AI/move_listing/move_max.c
	gcc -g -c -Wall AI/move_listing/move_max.c

move_listing.o: AI/move_listing/move_listing.c
	gcc -g -c -Wall AI/move_listing/move_listing.c

listing_checks.o: AI/move_listing/listing_checks/listing_checks.c
	gcc -g -c -Wall AI/move_listing/listing_checks/listing_checks.c

move_struct.o: AI/move_listing/move_struct/move_struct.c
	gcc -g -c -Wall AI/move_listing/move_struct/move_struct.c

rafle_listing.o: AI/move_listing/rafle_listing/rafle_listing.c
	gcc -g -c -Wall AI/move_listing/rafle_listing/rafle_listing.c

basic_play_function.o: graphic_interface/machine_plays_functions/basic_play_function.c
	gcc -g -c -Wall graphic_interface/machine_plays_functions/basic_play_function.c

precondition.o: fundamental_functions/game_functions/precondition/precondition.c
	gcc -g -c -Wall fundamental_functions/game_functions/precondition/precondition.c

debug_autoplay.o: graphic_interface/machine_plays_functions/debug_autoplay.c
	gcc -g -c -Wall graphic_interface/machine_plays_functions/debug_autoplay.c

debug.o: fundamental_functions/debug/debug.c
	gcc -g -c -Wall fundamental_functions/debug/debug.c
print_moves.o: AI/move_listing/move_listing_debug/print_moves.c
	gcc -g -c -Wall AI/move_listing/move_listing_debug/print_moves.c
