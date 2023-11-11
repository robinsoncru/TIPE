#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#define LG_WINDOW 640

typedef struct
{
    bool loaded; // text must be loaded only when it's necessary
    bool display;
    char *content;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect *rect;
    SDL_Color color;
} text;

typedef struct
{
    // RNG will be handled by another system
    // but will be initialized with the cache

    // Frame rate management
    Uint32 last_time;
    Uint32 time_now;

    // text displaying
    text *txtMessage;
    char **availableTexts;

    // window
    SDL_Window *window;
    SDL_Renderer *draw;
    SDL_Event event;
    bool is_playing;

    // rafle tree management
    bool display_tree;
    // PathTree* cacheTree;

    // error handling
    int statut;

    // ticks and time measurement
    // j'ai choisit cette implementation pour aisement
    // pouvoir rajouter des ticks de mesure si besoin
} GraphicCache;

void error()
{
    printf("check");
    fflush(stdout);
}

int main(int argc, char *argv[])
{
    // init the game

    // init the graphic cache
    GraphicCache *cache = malloc(sizeof(GraphicCache));
    cache->txtMessage = malloc(sizeof(text));

    // default values in case of an error in window initialisation
    cache->window = NULL;
    cache->draw = NULL;

    // intialisations that might fail

    // error handling in the initilization
    cache->statut = EXIT_FAILURE;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
    }
    else if (0 != TTF_Init())
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    }
    else if (0 != SDL_CreateWindowAndRenderer(LG_WINDOW, LG_WINDOW, SDL_WINDOW_RESIZABLE, &(cache->window), &(cache->draw)))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
    }
    else
    {
        cache->txtMessage->font = TTF_OpenFont("arial.ttf", 48);
        error();
        if (cache->txtMessage->font == NULL)
        {
            printf("Font pb");
        }
        else
        {
            cache->statut = EXIT_SUCCESS;
        }
    }

    if (cache->statut != EXIT_SUCCESS)

        printf("ok");
    TTF_Quit();
    SDL_Quit();
}