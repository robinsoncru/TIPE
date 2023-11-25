#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#ifndef INTERFACE_JEU_DAMES
#define INTERFACE_JEU_DAMES

#include "../game_functions/game_functions.h"
#include "../../rafle/calc/path_tree.h"

// Window pmetre
#define LG_WINDOW 640
#define FRAME 16
#define MAX_TICKS 20
#define LG_CASE (LG_WINDOW / NB_CASE_LG)


// Interface structure
typedef struct
{
    bool loaded; //text must be loaded only when it's necessary
    bool display;
    char* content;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *rect;
    // char string[LEN];
    SDL_Color color;
} text;


// Color constants
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
void drawLosange(SDL_Renderer *render, Case c, pawn p);
void selectPawn(Game *g, int x_mouse, int y_mouse);

// Init functions
void init_pawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int i, int init_place, int add, bool init_is_white);
void init_pawns(Game *g, bool init_is_white);
void init_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);
Game *create_game();

// Display functions
void display_damier(SDL_Renderer *render, Game *g);
void change_damier(Game *g);
void prepareText(SDL_Renderer *render, text *txt, char *string);
void printAndTurn(SDL_Renderer *render, text *txt, char *string, Game *g);

// Debug functions
void error();

// Memory functions
void free_game(Game *g);
#endif //MISC