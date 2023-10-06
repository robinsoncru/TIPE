#include <SDL2/SDL_ttf.h>
#include "game_functions_draughts.h"
// #include "../rafle_calc/rafle_calc.h"
// Window pmetre
#define LG_WINDOW 640
#define FRAME 16
#define MAX_TICKS 20
#define PREC LG_CASE
#define LG_CASE (LG_WINDOW / NB_CASE_LG)

#ifndef INTERFACE_STRUCT
#define INTERFACE_STRUCT
// #define LEN 200

// Interface structure

typedef struct
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *rect;
    // char string[LEN];
    SDL_Color color;
} text;
#endif //INTERFACE_STRUCT
//Color constants
extern SDL_Color orange;
extern SDL_Color blue;
extern SDL_Color green;
extern SDL_Color white;
extern SDL_Color black;
extern SDL_Color red;


// Geometric functions
void drawPoint(SDL_Renderer *render, SDL_Color color, int x, int y);
void drawLine(SDL_Renderer *render, SDL_Color color, int x0, int y0, int x1, int y1);
void drawRect(SDL_Renderer *render, SDL_Color color, const SDL_Rect rect);
void drawRects(SDL_Renderer *render, SDL_Color color, const SDL_Rect rect[], int len);
SDL_bool closeTo(int x0, int y0, int x1, int y1, int prec);
void drawLosange(SDL_Renderer *render, Case c, pawn p);
int selectPawn(Case damier[NB_CASE_LG][NB_CASE_LG], int x_mouse, int y_mouse, bool is_white);

// Modif for queen
bool changeQueenAllowed(pawn p, int lig, int col, Case damier[NB_CASE_LG][NB_CASE_LG]);
int queen_move(int x_mouse, int y_mouse, bool is_white, pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind);

// Init functions
void init_pawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white, int i, int init_place, int add);
void init_pawns(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white);
void init_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);

// Display functions
void change_damier(Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white);
void display_damier(SDL_Renderer *render, Case damier[NB_CASE_LG][NB_CASE_LG], pawn allPawns[2][NB_PAWNS]);
void prepareText(SDL_Renderer *render, text *txt, char *string);