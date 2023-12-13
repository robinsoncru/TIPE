#include "graphic_cache.h"
// graphic cache functions

typedef struct
{
    int k;
    char *s;
} assoc; // represente une association entre un entier et un string

// initializes the cache.
// In case of an error, interrupts the process
// and returns a cache with EXIT_FAILURE as a status
GraphicCache *initCache()
{

    // init aleat
    // j'ai choisi d'utiliser une cle predefinie pour initialiser
    // l'aleatoire, ca permet d'avoir des resultats reproductibles
    // utiles pour les tests des regles quantiques
    // srand(RNG_INIT_NBR);
    // J'ai besoin d'aleatoire car j'ai 15 fois le meme nombre consecutivement, met en comment si tu t'en sers pas stp
    srand(time(NULL));

    // init cache
    GraphicCache *cache = malloc(sizeof(GraphicCache));

    // frame variables don't need any initial values
    // cache -> time_now is intialized
    // cache -> last_time is initialized

    // init text
    cache->txtMessage = malloc(sizeof(text));
    cache->txtMessage->loaded = false;
    cache->txtMessage->display = false;
    cache->txtMessage->content = NULL;
    cache->txtMessage->font = NULL;
    cache->txtMessage->surface = NULL;
    cache->txtMessage->texture = NULL;
    cache->txtMessage->rect = malloc(sizeof(SDL_Rect));
    cache->txtMessage->color = red;

    assoc tmp[] = {
        {IND_CHANGE_ALLOWED, "pawn moved"},
        {IND_PB, "NO pawn moved"},
        {IND_NOTHING_HAPPENED, "You have nothing"},
        {IND_BAD_CHOICE, "ARG fuck !"},
        {IND_GLORY_QUEEN, "Yes putain !!!"}};
    int n = sizeof(tmp) / sizeof(assoc);
    cache->availableTexts = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        cache->availableTexts[tmp[i].k] = tmp[i].s;
    }

    // default values in case of an error in window initialisation
    cache->window = NULL;
    cache->draw = NULL;
    // res -> event has no default value
    cache->is_playing = true;

    // init rafle management
    cache->display_tree = false;
    // cache -> cacheTree = emptyTree;

    // Init the autoplay to false
    cache->autoplay = false;

    // init time measurement
    for (int i = 0; i < TICKS_ARRAY_LENGTH; i++)
    {
        cache->ticksArray[i] = 0;
    }

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
        if (cache->txtMessage->font == NULL)
        {
            printf("Font pb");
        }
        else
        {
            cache->statut = EXIT_SUCCESS;
        }
    }
    return cache;
}

void freeCache(GraphicCache *cache)
{
    // frame rate management freeing
    // cache->last_time no need to free
    // cache->time_now no need to free

    // text memory freeing
    // cache->txtMessage->display no need to free
    if (cache->txtMessage->texture != NULL)
        SDL_DestroyTexture(cache->txtMessage->texture);
    if (cache->txtMessage->surface != NULL)
        SDL_FreeSurface(cache->txtMessage->surface);
    if (cache->txtMessage->font != NULL)
        TTF_CloseFont(cache->txtMessage->font);
    if (NULL != cache->draw)
        SDL_DestroyRenderer(cache->draw);
    if (NULL != cache->window)
        SDL_DestroyWindow(cache->window);

    free(cache->txtMessage->rect);
    // cache->txtMessage->color no need to free
    free(cache->txtMessage);

    free(cache->availableTexts);
    // res->event no need to free
    // cache->is_playing no need to free

    // rafle tree management freeing
    // cache->display_tree no need to free
    // if (cache->cacheTree != emptyTree) {
    //     freeCurrentRafle(&(cache->cacheTree));
    // }

    // error handling freeing
    // cache->statut no need to free

    // time measurement management freeing
    // cache->ticksArray no need to free

    free(cache);
}

// called every tick to display stuff on the screen
// depending on the state of the cache
// DOES NOT modify the cache
void display(Game *g, GraphicCache *cache)
{
    display_damier(cache->draw, g);
    // if (cache->display_tree) {
    //     pathTreeDisplay(cache->draw, g->damier, cache->cacheTree);
    // }
    if (cache->txtMessage->display)
    {
        SDL_RenderCopy(cache->draw, cache->txtMessage->texture, NULL, cache->txtMessage->rect);
    }
}

// loads the text to display in the cache
void loadText(GraphicCache *cache, char *string)
{
    cache->txtMessage->loaded = true;
    prepareText(cache->draw, cache->txtMessage, string);
    cache->txtMessage->content = string;
}

void alert(GraphicCache *cache, int alertIndex, int timerIndex)
{
    loadText(cache, cache->availableTexts[alertIndex]);
    cache->txtMessage->display = true;
    // timer init
    Uint32 *timer = &(cache->ticksArray[timerIndex]);
    *timer = 1;
}