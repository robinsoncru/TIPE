#include "interface.h"

/*







    Color constants






*/

SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color red = {255, 0, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};
SDL_Color gold = {255, 215, 0, 255};
SDL_Color silver = {192, 192, 192, 255};

/*







    AUX FUNCTIONS






*/

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

/*







   Geometric functions






*/

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

void draw_little_square(SDL_Renderer *render, int dim, Case c, SDL_Color color)
{
    SDL_Rect fr;
    fr.x = c.rect.x + dim;
    fr.y = c.rect.y + dim;
    fr.w = c.rect.w - 2 * dim;
    fr.h = c.rect.h - 2 * dim;
    drawRect(render, color, fr);
}

void drawLosange(SDL_Renderer *render, Case c, pawn p, Game *g)
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

    int colory = p.color ? 255 - NUANCE_EFFECT*(p.pba-1) : NUANCE_EFFECT*(p.pba-1);
    // En fonction de la probabilite, le pion est plus ou moins clair ou fonce (selon qu'il soit noir ou blanc)

    for (int i = 0; i < 6; i++)
    {
        vertices[i].tex_coord.x = 1;
        vertices[i].tex_coord.y = 1;
        vertices[i].color.a = colory;
        vertices[i].color.r = colory;
        vertices[i].color.g = colory;
        vertices[i].color.b = colory;
    }

    SDL_RenderGeometry(render, NULL, vertices, 6, NULL, 0);

    if (!p.color)
    {

        SDL_SetRenderDrawColor(render, white.r, white.g, white.b, white.a);
        SDL_RenderDrawLine(render, vertices[0].position.x, vertices[0].position.y, vertices[1].position.x, vertices[1].position.y);
        SDL_RenderDrawLine(render, vertices[1].position.x, vertices[1].position.y, vertices[2].position.x, vertices[2].position.y);
        SDL_RenderDrawLine(render, vertices[2].position.x, vertices[2].position.y, vertices[5].position.x, vertices[5].position.y);
        SDL_RenderDrawLine(render, vertices[5].position.x, vertices[5].position.y, vertices[0].position.x, vertices[0].position.y);
    }

    if (p.queen)
    {
        draw_little_square(render, 20, c, blue);
    }

    if (p.friendly != -1)
    {
        draw_little_square(render, 30, c, orange);
    }

    if (p.ennemy != -1)
    {
        draw_little_square(render, 30, c, red);
    }

    if (c.ind_pawn == g->ind_move && c.pawn_color == g->is_white)
    {
        draw_little_square(render, 30, c, gold);
    }

    if (c.ind_pawn == g->ind_move_back && c.pawn_color == g->is_white)
    {
        draw_little_square(render, 30, c, silver);
    }
}

void selectPawn(Game *g, int x_mouse, int y_mouse)
{
    // printf("pt\n");
    if (g->is_white)
        y_mouse = LG_WINDOW - y_mouse;
    // printf("%d, %d\n", x_mouse, y_mouse);
    int lig = y_mouse / LG_CASE;
    int col = x_mouse / LG_CASE;
    printf("lig %d col %d\n", lig, col);
    // fflush(stdout);
    if (g->damier[lig][col].pawn_color == g->is_white)
    {
        g->ind_move = g->damier[lig][col].ind_pawn; // Return -1 if no pawn in the case
        // printf("Coord pion selec %d %d", g->allPawns[g->is_white][g->ind_move].lig, g->allPawns[g->is_white][g->ind_move].col);
        // flush();
    }
    else
        g->ind_move = -1;
}

/*







     Init functions






*/

void init_pawn(Game *g, Case damier[NB_CASE_LG][NB_CASE_LG], int i, int init_place, int add, bool init_is_white)
{
    if (init_is_white)
    {
        put_pawn_value(g, init_is_white, i, LIG, init_place);
        damier[init_place][add + 2 * i - init_place * NB_CASE_LG].pawn_color = true;
        put_pawn_value(g, init_is_white, i, COL, add + 2 * i - init_place * NB_CASE_LG);
    }
    else
    {
        put_pawn_value(g, init_is_white, i, LIG, NB_CASE_LG - init_place - 1);
        // pawns[i].lig = init_place;
        damier[NB_CASE_LG - init_place - 1][NON(add) + 2 * i - init_place * NB_CASE_LG].pawn_color = false;
        put_pawn_value(g, init_is_white, i, COL, NON(add) + 2 * i - init_place * NB_CASE_LG);
    }
    damier[get_pawn_value(g, init_is_white, i, LIG)][get_pawn_value(g, init_is_white, i, COL)].ind_pawn = i;
    put_pawn_value(g, init_is_white, i, COLOR, init_is_white);
}

/*{
    if (init_is_white)
    {
        pawns[i].lig = init_place;
        damier[init_place][add + 2 * i - init_place * NB_CASE_LG].pawn_color = true;
        pawns[i].col = add + 2 * i - init_place * NB_CASE_LG;
    }
    else
    {
        pawns[i].lig = NB_CASE_LG - init_place - 1;
        // pawns[i].lig = init_place;
        damier[NB_CASE_LG - init_place - 1][NON(add) + 2 * i - init_place * NB_CASE_LG].pawn_color = false;
        pawns[i].col = NON(add) + 2 * i - init_place * NB_CASE_LG;
    }
    damier[pawns[i].lig][pawns[i].col].ind_pawn = i;
    pawns[i].color = init_is_white;
}*/

void init_pawns(Game *g, bool init_is_white)
{
    int init_place = 0;
    for (int i = 0; i < NB_PAWNS; i++)
    // for (int i=0; i < NB_PAWNS/3; i++)
    {
        if (init_place % 2 == 0)
        {
            if (2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(g, g->damier, i, init_place, 0, init_is_white);
            // init_pawn(pawns, g.damier, g.is_white, i, init_place+2, 0);
            else
            {
                init_place++;
                init_pawn(g, g->damier, i, init_place, 1, init_is_white);
                // init_pawn(pawns, g.damier, g.is_white, i, init_place, 1);
            }
        }
        else
        {
            if (1 + 2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(g, g->damier, i, init_place, 1, init_is_white);
            else
            {
                init_place++;
                init_pawn(g, g->damier, i, init_place, 0, init_is_white);
            }
        }
        g->allPawns[init_is_white][i].alive = true;
        g->allPawns[init_is_white][i].queen = false;
        g->allPawns[init_is_white][i].friendly = -1;
        g->allPawns[init_is_white][i].ennemy = -1;
        g->allPawns[init_is_white][i].pba = 1;
    }

    // Initialize the rest of pawns with default pmetre and the good color
    for (int i = NB_PAWNS; i < 2 * NB_PAWNS; i++)
    {
        pawn_default_value_new(g, i, init_is_white);
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

Game *create_game()
{
    Game *g = malloc(sizeof(Game));
    g->indCheck = IND_NORMAL;
    g->is_white = true;
    init_damier(g->damier);

    init_pawns(g, true);
    init_pawns(g, false);
    g->nb_pawns[true] = NB_PAWNS;
    g->nb_pawns[false] = NB_PAWNS;

    g->ind_move = NEUTRAL_IND;
    g->ind_move_back = NEUTRAL_IND;

    g->coordForMoveBack.i = IND_LISTENING_MOVE_BACK;
    g->coordForMoveBack.j = IND_LISTENING_MOVE_BACK;
    return g;
}

/*







    Display functions






*/

void change_damier(Game *g)
{
    if (g->is_white)
    {
        for (int i = 0; i < NB_CASE_LG; i++)
        {
            for (int j = 0; j < NB_CASE_LG; j++)
            {
                g->damier[i][j].rect.y = LG_CASE * (NB_CASE_LG - i - 1);
            }
        }
    }
    else
    {
        for (int i = 0; i < NB_CASE_LG; i++)
        {
            for (int j = 0; j < NB_CASE_LG; j++)
            {
                g->damier[i][j].rect.y = LG_CASE * i;
            }
        }
    }
}

void display_damier(SDL_Renderer *render, Game *g)
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            if (g->damier[i][j].color)
                drawRect(render, white, g->damier[i][j].rect);
            else
                drawRect(render, black, g->damier[i][j].rect);

            if (!freeCase(g->damier[i][j]))
                drawLosange(render, g->damier[i][j], g->allPawns[g->damier[i][j].pawn_color][g->damier[i][j].ind_pawn], g);
        }
    }
}

// Prepare the text at displaying
void prepareText(SDL_Renderer *render, text *txt, char *string)
{
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

// Free the memory

void free_game(Game *g)
{
    // Je le laisse pour plus tard si on a des tableaux dynamiques (Victor G)
    free(g);
}