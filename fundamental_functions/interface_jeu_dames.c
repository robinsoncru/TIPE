#include <SDL2/SDL_ttf.h>
#include "game_functions_draughts.h"
// #include "rafle_calc/rafle_calc.h"
// Window pmetre
#define LG_WINDOW 640
#define FRAME 16
#define MAX_TICKS 20
#define PREC LG_CASE
#define LG_CASE (LG_WINDOW / NB_CASE_LG)
// #define LEN 200

// Compile : gcc interface_jeu_dames.c game_functions_draughts.c rafle_calc/rafle_calc.c rafle_calc/rafle.c $(sdl2-config --cflags --libs) -lSDL2_ttf -o dames

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

// Color constants

SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color red = {255, 0, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

// Aux functions

// SDL_Point *arrayToPoint(int tab_pts[][2], int len)
// {
//     static SDL_Point Points[NB_CASE];
//     for (int j = 0; j < len; j++)
//     {
//         Points[j].x = tab_pts[j][0];
//         Points[j].y = tab_pts[j][1];
//     }
//     return Points;
// }

// Geometric functions

void drawPoint(SDL_Renderer *render, SDL_Color color, int x, int y)
{
    // De meme: on fixe la couleur
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    // On dessine les elm
    SDL_RenderDrawPoint(render, x, y);
}

void drawLine(SDL_Renderer *render, SDL_Color color, int x0, int y0, int x1, int y1)
{
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(render, x0, y0, x1, y1);
}

void drawRect(SDL_Renderer *render, SDL_Color color, const SDL_Rect rect)
{
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(render, &rect);
    // printf("%d %d %d %d", rect.h, rect.w, rect.x, rect.y);
}

void drawRects(SDL_Renderer *render, SDL_Color color, const SDL_Rect rect[], int len)
{
    SDL_SetRenderDrawColor(render, color.r, color.g, color.b, color.a);
    SDL_RenderFillRects(render, rect, len);
}

SDL_bool closeTo(int x0, int y0, int x1, int y1, int prec)
{
    SDL_Point pt = {x0, y0};
    SDL_Rect rect = {x1 - (prec / 2), y1 - (prec / 2), prec, prec};
    return SDL_PointInRect(&pt, &rect);
}

void drawLosange(SDL_Renderer *render, Case c, pawn p)
{

    SDL_Vertex vertices[6];

    vertices[0].position.x = c.rect.x;
    vertices[0].position.y = c.rect.y + LG_CASE / 2;
    vertices[1].position.x = c.rect.x + LG_CASE / 2;
    ;
    vertices[1].position.y = c.rect.y;
    vertices[2].position.x = c.rect.x + LG_CASE;
    vertices[2].position.y = c.rect.y + LG_CASE / 2;
    vertices[3].position.x = c.rect.x;
    vertices[3].position.y = c.rect.y + LG_CASE / 2;
    vertices[4].position.x = c.rect.x + LG_CASE;
    vertices[4].position.y = c.rect.y + LG_CASE / 2;
    vertices[5].position.x = c.rect.x + LG_CASE / 2;
    vertices[5].position.y = c.rect.y + LG_CASE;

    if (p.color)
    {

        for (int i = 0; i < 6; i++)
        {
            vertices[i].tex_coord.x = 1;
            vertices[i].tex_coord.y = 1;
            vertices[i].color.a = 255;
            vertices[i].color.r = 255;
            vertices[i].color.g = 255;
            vertices[i].color.b = 255;
        }

        SDL_RenderGeometry(render, NULL, vertices, 6, NULL, 0);
    }
    else
    {

        SDL_SetRenderDrawColor(render, white.r, white.g, white.b, white.a);
        SDL_RenderDrawLine(render, vertices[0].position.x, vertices[0].position.y, vertices[1].position.x, vertices[1].position.y);
        SDL_RenderDrawLine(render, vertices[1].position.x, vertices[1].position.y, vertices[2].position.x, vertices[2].position.y);
        SDL_RenderDrawLine(render, vertices[2].position.x, vertices[2].position.y, vertices[5].position.x, vertices[5].position.y);
        SDL_RenderDrawLine(render, vertices[5].position.x, vertices[5].position.y, vertices[0].position.x, vertices[0].position.y);
    }

    if (p.queen)
    {
        SDL_Rect qr;
        qr.x = c.rect.x + 20;
        qr.y = c.rect.y + 20;
        qr.w = c.rect.w - 40;
        qr.h = c.rect.h - 40;
        drawRect(render, red, qr);
    }
}

int selectPawn(Case damier[NB_CASE_LG][NB_CASE_LG], int x_mouse, int y_mouse, bool is_white)
{
    // printf("pt\n");
    if (is_white)
        y_mouse = LG_WINDOW - y_mouse;
    // printf("%d, %d\n", x_mouse, y_mouse);
    int lig = y_mouse / LG_CASE;
    int col = x_mouse / LG_CASE;
    printf("lig %d col %d\n", lig, col);
    fflush(stdout);
    if (damier[lig][col].pawn_color == is_white)
        return damier[lig][col].ind_pawn; // Return NEUTRAL_IND if no pawn in the case
    else
        return NEUTRAL_IND;
}


// Init functions

void init_pawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white, int i, int init_place, int add)
{
    if (is_white)
    {
        pawns[i].lig = init_place;
        damier[init_place][add + 2 * i - init_place * NB_CASE_LG].pawn_color = true;
        pawns[i].col = add + 2 * i - init_place * NB_CASE_LG;
    }
    else
    {
        pawns[i].lig = NB_CASE_LG - init_place - 1;
        damier[NB_CASE_LG - init_place - 1][NON(add) + 2 * i - init_place * NB_CASE_LG].pawn_color = false;
        pawns[i].col = NON(add) + 2 * i - init_place * NB_CASE_LG;
    }
    damier[pawns[i].lig][pawns[i].col].ind_pawn = i;
    pawns[i].color = is_white;
}

void init_pawns(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white)
{
    int init_place = 0;
    for (int i = 0; i < NB_PAWNS; i++)
    // for (int i=0; i < NB_PAWNS/3; i++)
    {
        if (init_place % 2 == 0)
        {
            if (2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(pawns, damier, is_white, i, init_place, 0);
            // init_pawn(pawns, damier, is_white, i, init_place+2, 0);
            else
            {
                init_place++;
                init_pawn(pawns, damier, is_white, i, init_place, 1);
                // init_pawn(pawns, damier, is_white, i, init_place, 1);
            }
        }
        else
        {
            if (1 + 2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(pawns, damier, is_white, i, init_place, 1);
            else
            {
                init_place++;
                init_pawn(pawns, damier, is_white, i, init_place, 0);
            }
        }
        pawns[i].alive = true;
        pawns[i].queen = false;
    }
}

void init_damier(Case damier[NB_CASE_LG][NB_CASE_LG])
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            damier[i][j].rect.x = LG_CASE * j;
            damier[i][j].rect.y = LG_CASE * (NB_CASE_LG - i - 1);
            damier[i][j].rect.w = LG_CASE;
            damier[i][j].rect.h = LG_CASE;
            damier[i][j].pawn_color = false;
            damier[i][j].ind_pawn = -1;
            if ((i + j) % 2 == 0)
                damier[i][j].color = false;
            else
                damier[i][j].color = true;
        }
    }
}

// Display functions

void change_damier(Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white)
{
    if (is_white)
    {
        for (int i = 0; i < NB_CASE_LG; i++)
        {
            for (int j = 0; j < NB_CASE_LG; j++)
            {
                damier[i][j].rect.y = LG_CASE * (NB_CASE_LG - i - 1);
            }
        }
    }
    else
    {
        for (int i = 0; i < NB_CASE_LG; i++)
        {
            for (int j = 0; j < NB_CASE_LG; j++)
            {
                damier[i][j].rect.y = LG_CASE * i;
            }
        }
    }
}

void display_damier(SDL_Renderer *render, Case damier[NB_CASE_LG][NB_CASE_LG], pawn allPawns[2][NB_PAWNS])
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            if (damier[i][j].color)
                drawRect(render, white, damier[i][j].rect);
            else
                drawRect(render, black, damier[i][j].rect);
            if (!freeCase(damier[i][j]))
            {
                drawLosange(render, damier[i][j], allPawns[damier[i][j].pawn_color][damier[i][j].ind_pawn]);
            }
        }
    }
}

void prepareText(SDL_Renderer *render, text *txt, char *string)
{ // Prepare the text at displaying
    txt->surface = TTF_RenderText_Solid(txt->font, string, txt->color);
    txt->texture = SDL_CreateTextureFromSurface(render, txt->surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(txt->texture, NULL, NULL, &texW, &texH);
    txt->rect->x = (LG_WINDOW - texW) / 2;
    txt->rect->y = (LG_WINDOW - texH) / 2;
    txt->rect->w = texW;
    txt->rect->h = texH;
}