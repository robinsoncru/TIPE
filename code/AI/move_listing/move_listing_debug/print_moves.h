#ifndef DEBUG_AUTOPLAY
#define DEBUG_AUTOPLAY

#include "../move_struct/move_struct.h"
#include "../../../fundamental_functions/debug/debug.h"
#include <stdlib.h>

void print_move(Move m);
void print_moves(MoveTab *t);
void print_moves_write_file_while_endgame(Game *g, MoveTab *t);
void print_move_write_file_while_endgame(Game *g, Move m);

#endif