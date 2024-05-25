#ifndef FGRAPHIC
#define FGRAPHIC

#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

//time management
#define TICKS_ARRAY_LENGTH 2 //number of chronometers
//ticks for text displaying
#define CHANGE_TICKS 0
#define ERROR_TICKS 1



// affichage structure
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

typedef struct
{
    //RNG will be handled by another system
    //but will be initialized with the cache

    //Frame rate management
    Uint32 last_time;
    Uint32 time_now;

    //text displaying
    text* txtMessage;
    char** availableTexts;

    //window
    SDL_Window* window;
    SDL_Renderer *draw;
    SDL_Event event;
    bool is_playing;

    //rafle tree management
    bool display_tree;
    //PathTree* cacheTree;

    //error handling
    int statut;

    //ticks and time measurement
    //j'ai choisit cette implementation pour aisement
    //pouvoir rajouter des ticks de mesure si besoin
    Uint32 ticksArray[TICKS_ARRAY_LENGTH];

    bool autoplay;
    // Lorsque autoplay est vrai, joue automatiquement le coup predefini dans play_a_move dans autoplay_function
} GraphicCache;

#endif