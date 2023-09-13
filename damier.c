#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL_ttf.h>
#define LG_WINDOW 640
#define NB_CASE_LG 8
#define NB_CASE NB_CASE_LG *NB_CASE_LG
#define LG_CASE (LG_WINDOW / NB_CASE_LG)
#define NB_PAWNS (NB_CASE_LG)
#define FRAME 16
#define PREC LG_CASE
#define MAX_TICKS 20
#define IND_PB -3
#define IND_CHANGE_ALLOWED -2
#define NEUTRAL_IND -1
#define LEFT_BACK 0
#define LEFT_FORWARD 1
#define RIGHT_BACK 2
#define RIGHT_FORWARD 3
// #define LEN 200

// Compile : gcc damier.c $(sdl2-config --cflags --libs) -lSDL2_ttf -o a && ./a

typedef struct
{
    int occupied;
    SDL_Rect rect;
    bool color;
} Case;

typedef struct Rafle Rafle;
struct Rafle {
    int ind_eat;
    Rafle *pt;
};

typedef struct Liste Liste;
struct Liste {Rafle *first;};

typedef struct
{
    int lig, col, alive;
    bool queen;
} pawn;

typedef struct
{
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *rect;
    // char string[LEN];
    SDL_Color color;
} text;

SDL_Color orange = {255, 127, 40, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color red = {255, 0, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color white = {255, 255, 255, 255};
SDL_Color black = {0, 0, 0, 255};

bool pawnAlive(pawn p)
{
    return (p.alive != 0);
}

Rafle *createRafle()
{
    Rafle *rafle = malloc(sizeof(Rafle));
    rafle->pt = NULL;
    rafle->ind_eat = -1;
    return rafle;
}

bool isEmpty(Rafle *rafle)
{
    return (rafle->pt == NULL);
}

void addRafle(Rafle *rafle, int ind_eat)
{
    rafle->ind_eat = ind_eat;
    rafle->pt = createRafle();
}

void destroyRafle(Rafle *rafle)
{
    Rafle *rafle_a_sup;
    while (!isEmpty(rafle)) {
        rafle_a_sup = rafle;
        rafle = rafle->pt;
        free(rafle_a_sup);
    }
    rafle->ind_eat = -1;
}

// void printRafle(Rafle *rafle) {
//     while (!isEmpty(rafle)) {
//         rafle_a_sup = rafle;
//         rafle = rafle->pt;
//         free(rafle_a_sup);
//     }
// }

void delete_liste(Liste *liste) {
	// Libère proprement tous les maillons
	for (Rafle *Maillon_present = liste->first; Maillon_present != NULL; Maillon_present = Maillon_present->pt) {
		free(Maillon_present);
	}
}

SDL_Point *arrayToPoint(int tab_pts[][2], int len)
{
    static SDL_Point Points[NB_CASE];
    for (int j = 0; j < len; j++)
    {
        Points[j].x = tab_pts[j][0];
        Points[j].y = tab_pts[j][1];
    }
    return Points;
}

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

SDL_bool closeTo(int x0, int y0, int x1, int y1, int prec)
{
    SDL_Point pt = {x0, y0};
    SDL_Rect rect = {x1 - (prec / 2), y1 - (prec / 2), prec, prec};
    return SDL_PointInRect(&pt, &rect);
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

int NON(int b)
{
    return (b + 1) % 2;
}

void init_pawn(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white, int i, int init_place, int add)
{
    if (is_white)
    {
        pawns[i].lig = init_place;
        damier[init_place][add + 2 * i - init_place * NB_CASE_LG].occupied = 1;
        pawns[i].col = add + 2 * i - init_place * NB_CASE_LG;
        pawns[i].alive = 1;
    }
    else
    {
        pawns[i].lig = NB_CASE_LG - init_place - 1;
        damier[NB_CASE_LG - init_place - 1][NON(add) + 2 * i - init_place * NB_CASE_LG].occupied = 2;
        pawns[i].col = NON(add) + 2 * i - init_place * NB_CASE_LG;
        pawns[i].alive = 2;
    }
}

void init_pawns(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], bool is_white)
{
    int init_place = 0;
    for (int i = 0; i < NB_PAWNS; i++)
    {
        if (init_place % 2 == 0)
        {
            if (2 * i - init_place * NB_CASE_LG < NB_CASE_LG)
                init_pawn(pawns, damier, is_white, i, init_place, 0);
            else
            {
                init_place++;
                init_pawn(pawns, damier, is_white, i, init_place, 1);
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
        pawns[i].queen = false;
    }
}

void print_pawns(pawn pawns[])
{
    for (int i = 0; i < NB_PAWNS; i++)
    {
        printf("Sel %d\n", pawns[i].alive);
    }
}

int pawn_move(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, bool gauche)
{
    printf("call pawn_move\n");
    if (ind > -1 && pawnAlive(pawns[ind]))
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d", i, j);
        if (pawns[ind].alive == 1 && i < NB_CASE_LG - 1)
        {
            if (gauche && j > 0 && damier[i + 1][j - 1].occupied == 0)
            {
                damier[i + 1][j - 1].occupied = 1;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i + 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && damier[i + 1][j + 1].occupied == 0)
            {
                damier[i + 1][j + 1].occupied = 1;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i + 1;
                pawns[ind].col = j + 1;
            }
            else
            {
                printf("white ind pb occ\n");
                return IND_PB;
            }
        }
        else if (i > 0)
        {
            if (gauche && j > 0 && damier[i - 1][j - 1].occupied == 0)
            {
                damier[i - 1][j - 1].occupied = 2;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i - 1;
                pawns[ind].col = j - 1;
            }
            else if (!gauche && j < NB_CASE_LG - 1 && damier[i - 1][j + 1].occupied == 0)
            {
                damier[i - 1][j + 1].occupied = 2;
                damier[i][j].occupied = 0;
                pawns[ind].lig = i - 1;
                pawns[ind].col = j + 1;
            }

            // printf("ind %d\n", ind);
            // printf("%d %d\n", i, j);
            // printf("%d %d\n", pawns[ind].lig, pawns[ind].col);

            else
            {
                printf("black idn pb occ\n");
                return IND_PB;
            }
        }
        else
        {
            printf("color pb\n");
            return IND_PB;
        }
        return IND_CHANGE_ALLOWED;
    }
    printf("pawn alive %d or ind = %d", pawnAlive(pawns[ind]), ind);
    return IND_PB;
}

int giveIndPawn(int lig, int col, pawn pawns[])
{
    for (int i = 0; i < NB_PAWNS; i++)
    {
        if (pawns[i].lig == lig && pawns[i].col == col && pawnAlive(pawns[i]))
        {
            return i;
        }
    }
    return -1;
}

bool canEat(pawn pawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    return (damier[i + 2 * add0][j + 2 * add1].occupied == 0 && damier[i + add0][j + add1].occupied == NON(pawns[ind].alive - 1) + 1);
}

int changeForEat(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind, int i, int j, int add0, int add1)
{
    assert(ind > -1);
    damier[i + add0][j + add1].occupied = 0;
    damier[i][j].occupied = 0;
    damier[i + 2 * add0][j + 2 * add1].occupied = pawns[ind].alive;
    printf("pawn which is eaten %d\n", giveIndPawn(i + add0, j + add1, Npawns));

    Npawns[giveIndPawn(i + add0, j + add1, Npawns)].alive = 0;
    pawns[ind].lig = i + 2 * add0;
    pawns[ind].col = j + 2 * add1;
    printf("change allowed %d %d", i + 2 * add0, j + 2 * add1);
    return IND_CHANGE_ALLOWED;
}

int eatPawn(pawn pawns[], pawn Npawns[], Case damier[NB_CASE_LG][NB_CASE_LG], int ind)
{
    printf("call EatPawn \nind pawn which eat %d\n", ind);
    if (ind > -1)
    {
        int i = pawns[ind].lig;
        int j = pawns[ind].col;
        // printf("%d %d move %d NcanEat %d", i, j, move_direction, canEat(pawns, damier, ind, i, j, 1, -1));
        printf("%d %d", i, j);

        if (i > 1 && j > 1 && canEat(pawns, damier, ind, i, j, -1, -1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, -1, -1);
        else if (i < NB_CASE_LG - 2 && j > 1 && canEat(pawns, damier, ind, i, j, 1, -1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, 1, -1);
        else if (i > 1 && j < NB_CASE_LG - 2 && canEat(pawns, damier, ind, i, j, -1, 1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, -1, 1);
        else if (i < NB_CASE_LG - 2 && j < NB_CASE_LG - 2 && canEat(pawns, damier, ind, i, j, 1, 1))
            return changeForEat(pawns, Npawns, damier, ind, i, j, 1, 1);
        else
        {
            printf("no condition satisfied");
            return IND_PB;
        }
    }
    printf("ind = -1\n");
    return NEUTRAL_IND;
}

void drawLosange(SDL_Renderer *render, Case c)
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

    if (c.occupied == 1)
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
}

void print_damier(Case damier[NB_CASE_LG][NB_CASE_LG])
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            printf("Case ligne %d col %d pos: %d, %d\n", i, j, damier[i][j].rect.x, damier[i][j].rect.y);
        }
    }
}

int selectPawn(pawn pawns[], int x_mouse, int y_mouse, bool is_white)
{
    // printf("pt\n");
    if (is_white)
        y_mouse = LG_WINDOW - y_mouse;
    // printf("%d, %d\n", x_mouse, y_mouse);
    for (int i = 0; i < NB_PAWNS; i++)
    {
        // printf("index %d\n", i);
        // printf("CASE %d %d\n", pawns[i].col * LG_CASE + LG_CASE / 2, pawns[i].lig * LG_CASE + LG_CASE / 2);
        if (closeTo(x_mouse, y_mouse, pawns[i].col * LG_CASE + LG_CASE / 2, pawns[i].lig * LG_CASE + LG_CASE / 2, PREC) && pawnAlive(pawns[i]))
        {
            // printf("index %d\n", i);
            // printf("CASE %d %d\n", pawns[i].col * LG_CASE + LG_CASE / 2, pawns[i].lig * LG_CASE + LG_CASE / 2);
            return i;
        }
    }
    return NEUTRAL_IND;
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
            damier[i][j].occupied = 0;
            if ((i + j) % 2 == 0)
                damier[i][j].color = false;
            else
                damier[i][j].color = true;
        }
    }
}

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

void display_damier(SDL_Renderer *render, Case damier[NB_CASE_LG][NB_CASE_LG])
{
    for (int i = 0; i < NB_CASE_LG; i++)
    {
        for (int j = 0; j < NB_CASE_LG; j++)
        {
            if (damier[i][j].color)
                drawRect(render, white, damier[i][j].rect);
            else
                drawRect(render, black, damier[i][j].rect);
            if (damier[i][j].occupied != 0)
            {
                drawLosange(render, damier[i][j]);
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

int main(int argc, char *argv[])
{

    // Init game

    Case damier[NB_CASE_LG][NB_CASE_LG];
    int ind_move = NEUTRAL_IND;
    bool is_white = true;
    pawn allPawns[2][NB_PAWNS];
    Rafle *rafle = createRafle();
    int allMoves[4][2] = {{LEFT_FORWARD, LEFT_BACK}, {LEFT_BACK, LEFT_FORWARD}, {RIGHT_FORWARD, RIGHT_BACK}, {RIGHT_BACK, RIGHT_FORWARD}};

    init_damier(damier);

    init_pawns(allPawns[1], damier, true);
    init_pawns(allPawns[0], damier, false);

    // print_pawns(whites);
    // print_pawns(blacks);
    // print_damier(damier);
    // printf("Lg case %d\n", LG_CASE);

    // Init window

    int statut = EXIT_FAILURE;
    SDL_Window *window = NULL;
    SDL_Renderer *draw = NULL;
    SDL_Event event;

    // Init text
    text *txtMessage = malloc(sizeof(text));
    txtMessage->font = NULL;
    txtMessage->surface = NULL;
    txtMessage->texture = NULL;
    txtMessage->rect = malloc(sizeof(SDL_Rect));
    txtMessage->color = red;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    if (0 != TTF_Init())
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        goto Quit;
    }

    if (0 != SDL_CreateWindowAndRenderer(LG_WINDOW, LG_WINDOW, SDL_WINDOW_RESIZABLE, &window, &draw))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        goto Quit;
    }

    txtMessage->font = TTF_OpenFont("arial.ttf", 48);

    if (txtMessage->font == NULL)
    {
        printf("Font pb");
        goto Quit;
    }

    bool is_playing = true;
    Uint32 last_time = SDL_GetTicks();
    Uint32 time_now;
    Uint32 change_ticks = 0;
    Uint32 error_ticks = 0;

    // Start the game

    while (is_playing)
    {

        time_now = SDL_GetTicks();

        if (time_now - last_time > FRAME)
        {
            last_time = time_now;

            SDL_RenderClear(draw);

            // Drawing part

            // For each turn, need to change the screen's display
            if (change_ticks > MAX_TICKS)
            {
                is_white = !is_white;
                change_damier(damier, is_white);
                change_ticks = 0;
            }

            if (error_ticks > MAX_TICKS)
                error_ticks = 0;

            // Draw the board in the screen
            display_damier(draw, damier);

            // Create a transition effect
            if (change_ticks > 0)
            {
                change_ticks++;
                // display the text
                SDL_RenderCopy(draw, txtMessage->texture, NULL, txtMessage->rect);
            }

            if (error_ticks > 0)
            {
                SDL_RenderCopy(draw, txtMessage->texture, NULL, txtMessage->rect);
                error_ticks++;
            }

            /* Rafraichit l'écran */
            SDL_RenderPresent(draw);

            // Control pannel

            if (SDL_PollEvent(&event) == 1)
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    is_playing = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    ind_move = selectPawn(allPawns[is_white], event.button.x, event.button.y, is_white);
                    if (ind_move == NEUTRAL_IND)
                        printf("No pawn selected");
                    // printf("ind_move %d", ind_move);
                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_LEFT)
                        ind_move = pawn_move(allPawns[is_white], damier, ind_move, true);
                    else if (event.key.keysym.sym == SDLK_RIGHT)
                        ind_move = pawn_move(allPawns[is_white], damier, ind_move, false);
                    else if (event.key.keysym.sym == SDLK_UP)
                        ind_move = eatPawn(allPawns[is_white], allPawns[!is_white], damier, ind_move);
                    else if (event.key.keysym.sym == SDLK_ESCAPE)
                        is_playing = false;
                    else
                        ind_move = NEUTRAL_IND;
                    break;
                }
            }

            // Check if the move is allowed

            if (ind_move == IND_CHANGE_ALLOWED)
            {
                ind_move = NEUTRAL_IND;
                change_ticks++;
                prepareText(draw, txtMessage, "pawn moved");
            }
            else if (ind_move == IND_PB)
            {
                // printf("No pawn moved");
                prepareText(draw, txtMessage, "NO pawn moved");
                ind_move = NEUTRAL_IND;
                error_ticks++;
            }
        }
        // is_playing=false;
    }

    statut = EXIT_SUCCESS;
    // printf("change \n");
    // print_damier(damier);

Quit:
    // Free the resources
    destroyRafle(rafle);
    if (txtMessage->texture != NULL)
        SDL_DestroyTexture(txtMessage->texture);
    if (txtMessage->surface != NULL)
        SDL_FreeSurface(txtMessage->surface);
    if (txtMessage->font != NULL)
        TTF_CloseFont(txtMessage->font);
    if (NULL != draw)
        SDL_DestroyRenderer(draw);
    if (NULL != window)
        SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return statut;
}