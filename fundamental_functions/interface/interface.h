#ifndef INTERFACE_JEU_DAMES
#define INTERFACE_JEU_DAMES

#include <SDL2/SDL_ttf.h>

#include "../game_functions/Source/source_header.h"
#include "../game_functions/game_functions.h"
#include "../../rafle/calc/path_tree.h"
#include "../../rafle/rafle_choice/path_struct.h"

// Window pmetre
#define LG_WINDOW 640
#define FRAME 16
#define MAX_TICKS 10
#define LG_CASE (LG_WINDOW / NB_CASE_LG)

// Color pmetre
#define NUANCE_EFFECT 20


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
void drawLosange(SDL_Renderer *render, Case c, pawn p, Game *g);
void selectPawn(Game *g, int x_mouse, int y_mouse);

// Init functions
void init_pawn(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int i, int init_place, int add, bool init_is_white);
void init_pawns(Game *g, bool init_is_white);
void init_damier(Case damier[NB_CASE_LG][NB_CASE_LG]);
Game *create_game();

// Display functions
void display_damier(SDL_Renderer *render, Game *g);
void change_damier(Game *g);
void prepareText(SDL_Renderer *render, text *txt, char *string);

#endif //INTERFACE_JEU_DAMES